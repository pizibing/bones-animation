#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "../SimpleLine.h"
#include "ChSkeletonInstance.h"
#include "ChBoneInstance.h"
#include "ChSkeleton.h"
#include "ChBone.h"
#include "ChAnimationManager.h"
#include "ChAnimation.h"
#include <assert.h>

ChSkeletonInstance::ChSkeletonInstance(ChSkeleton* skeleton)
{
	assert(skeleton);
	m_skeleton = skeleton;
	bones = new ChBoneInstance*[skeleton->getBoneNum()];
	ChBone * chbone;
	for(int i=0;i<skeleton->getBoneNum();i++){
		bones[i] = new ChBoneInstance(i);
		chbone =  m_skeleton->getBone(bones[i]->getId());
		// set inverse matrix
		bones[i]->setInverse(chbone->getBindPoseInverse());
		// set static transform from bone
		bones[i]->setRotation(chbone->getRotation());
		Vector3D v = chbone->getTranslation();
		bones[i]->setTranslation(chbone->getTranslation());
	}
	root = bones[skeleton->getRootBone()->getId()];
	calculateAbsoluteTransform(root->getId());

	// init boneLine array to NULL
	boneLines = NULL;
}

ChSkeletonInstance::~ChSkeletonInstance(void)
{
	for(int i=0;i<m_skeleton->getBoneNum();i++){
		delete bones[i];
	}
	if(bones)delete [] bones;
	bones = NULL;
	if(boneLines) delete [] boneLines;
	boneLines = NULL;
}

//get function of root
ChBoneInstance* ChSkeletonInstance::getRoot(){
	return root;
}

// this function will calculate the current position of all the bone
// instance of this skeleton.
// this function will return a matrix that describes the way the character
// should change in world space.(this matrix is produced by the move of 
// root bone)
// animanager where you can get all animation
// animation is the name of the animation to use, aimationtime is the 
// current play time in animation
Matrix ChSkeletonInstance::calSkeletonInstance(ChAnimationManager * animanager, int animationtime, const char* animation){
	// set root bone's matrix's rotation according to animation, but translation to default identity
	// set rotation
	root->setRotation(animanager->blendAnimationBoneRotation(animationtime,animation,m_skeleton->getRootBone()->getName()));
	// for all bones except root bone, set matrix to relative matrix this animation time
	for(int i=1;i<m_skeleton->getBoneNum();i++){
		//bones[i]->setMatrix(animanager->blendAnimationBone(animationtime,animation,m_skeleton->getBone(i)->getName()));
		bones[i]->setRotation(animanager->blendAnimationBoneRotation(animationtime,animation,m_skeleton->getBone(i)->getName()));
		bones[i]->setTranslation(animanager->blendAnimationBoneTranslation(animationtime,animation,m_skeleton->getBone(i)->getName()));
	}
	calculateAbsoluteTransform(m_skeleton->getRootBone()->getId());

	// return the change matrix
	return calRootChange(animanager,animationtime,animation);
}

// this function will calculate the current position of all the bone
// instance of this skeleton.
// this function will return a matrix that describes the way the character
// should change in world space.(this matrix is produced by the move of 
// root bone)
// animanager where you can get all animation
// animation1 and animation2 are the names of the animation to use,
// this function will blend these two animations
// animationtime1 and animationtime2 are the current play time of animation1
// and animation2
// power1 is the power of animation1 in this blend, it should be 0 to 1
// power of animation2 is of cause 1 minus power1
Matrix ChSkeletonInstance::calSkeletonInstance(ChAnimationManager * animanager, int animationtime1,
												 int animationtime2, const char* animation1, const char* animation2
												 , float power1){
	// power1 should be 0 to 1
    assert(power1 > 0);
	assert(power1 < 1);

	// set root bone's matrix's rotation according to animation, but translation to default identity
	// set rotation
	Quaternion r1 = animanager->blendAnimationBoneRotation(animationtime1,animation1,m_skeleton->getRootBone()->getName());
	Quaternion r2 = animanager->blendAnimationBoneRotation(animationtime2,animation2,m_skeleton->getRootBone()->getName());
	root->setRotation(Quaternion::slerp(r1,r2,1.0f-power1));

	// for all bones except root bone, set matrix to relative matrix this animation time
	for(int i=1;i<m_skeleton->getBoneNum();i++){
		//Matrix m1 = animanager->blendAnimationBone(animationtime1,animation1,m_skeleton->getBone(i)->getName());
		//Matrix m2 = animanager->blendAnimationBone(animationtime2,animation2,m_skeleton->getBone(i)->getName());
		//bones[i]->setMatrix(m1*power1+m2*(1-power1));

		Quaternion r1 = animanager->blendAnimationBoneRotation(animationtime1,animation1,m_skeleton->getBone(i)->getName());
		Quaternion r2 = animanager->blendAnimationBoneRotation(animationtime2,animation2,m_skeleton->getBone(i)->getName());
		bones[i]->setRotation(Quaternion::slerp(r1,r2,1.0f-power1));

		Vector3D t1 = animanager->blendAnimationBoneTranslation(animationtime1,animation1,m_skeleton->getBone(i)->getName());
		Vector3D t2 = animanager->blendAnimationBoneTranslation(animationtime2,animation2,m_skeleton->getBone(i)->getName());
		bones[i]->setTranslation(t1*power1+t2*(1.0f-power1));
		
	}
	calculateAbsoluteTransform(m_skeleton->getRootBone()->getId());

	// calculate change matrix factoring by power
	Matrix change1 = calRootChange(animanager,animationtime1,animation1);
	Matrix change2 = calRootChange(animanager,animationtime2,animation2);
	Matrix change = change1 * power1 + change2 * (1 - power1);

	return change;
}

// calculate the root change matrix at the given animationtime
// in animation in animanager
Matrix ChSkeletonInstance::calRootChange(ChAnimationManager * animanager, 
					 int animationtime, const char* animation){
	
	// get current transform matrix of root
	Matrix current = Matrix();
	current.set(Quaternion(),//animanager->getCurrentRootRotation(animationtime,animation),
	 animanager->getCurrentRootTranslation(animationtime,animation));
	// get last transform matrix of root
	Matrix lastm = Matrix();
	lastm.set(Quaternion(),//animanager->getLastRootRotation(animationtime,animation),
	 animanager->getLastRootTranslation(animationtime,animation));
	// get inverse matrix of last matrix
	Matrix inverseLast = lastm.getInverseMatrix();

	// return current * inverse of last
	return current * inverseLast;
}

// get a bone instance from the bones whose id equals to the given id
ChBoneInstance* ChSkeletonInstance::getBoneInstance(int id){
	assert(id>=0&&id<m_skeleton->getBoneNum());
	return bones[id];
}

// get a bone instance position in world space from the bones whose id equals to the given id
const Vector3D & ChSkeletonInstance::getBoneInstancePosition(int id) const{
	assert(id>=0&&id<m_skeleton->getBoneNum());
	return  bones[id]->getAbsoluteTranslation();
}

// calculate absolute transform for all bone instance 
// with boneId as root bone
void ChSkeletonInstance::calculateAbsoluteTransform(int boneId){
	ChBone * bone = m_skeleton->getBone(boneId);
	ChBone * parent = bone->getParentBone();
	// calculate transform in world space for bone instance with boneId
	if(parent){
		// this bone's absolute transform = this bone's relative transform * parent bone's absolute transform
		//bones[boneId]->setMatrix(bones[boneId]->getMatrix()*bones[parent->getId()]->getMatrix());

		// this bone's absolute rotation = parent bone's absolute rotation * this bone's relative rotation
		bones[boneId]->setAbsoluteRotation(bones[parent->getId()]->getAbsoluteRotation() * bones[boneId]->getRotation());
		// this bone's absolute translation = parent bone's absolute rotation * this bone's relative translation + parent bone's translation 
		bones[boneId]->setAbsoluteTranslation( bones[parent->getId()]->getAbsoluteRotation() * bones[boneId]->getTranslation() 
									+ bones[parent->getId()]->getAbsoluteTranslation() );
	}
	else {	
		// this is a root bone, absolute transform = relative transform
		bones[boneId]->setAbsoluteRotation(bones[boneId]->getRotation());
		bones[boneId]->setAbsoluteTranslation(bones[boneId]->getTranslation());
	}
	ChBone ** childbones = bone->getAllChildBones();
	for(int i=0;i<bone->getChildBoneNum();i++){
		calculateAbsoluteTransform(childbones[i]->getId());
	}
}

// get the lines for the skeleton
// modify num to get the line number
SimpleLine * ChSkeletonInstance::getBoneLines(int *num, const Matrix & chMatrixInstance){
	if(!boneLines){
		// initial bone line array
		// root bone has no line
		boneLines = new SimpleLine[m_skeleton->getBoneNum()-1];
		
	}
	int lineNum = 0;
	ChBoneInstance * bone;
	ChBoneInstance * parent;
	int parentId;
	Vector3D v;
	// generate lines for all bone instance except root bone
	for(int i=0;i<m_skeleton->getBoneNum();i++){
		bone = bones[i];
		// get parent bone
		parentId = m_skeleton->getBone(i)->getParentId();
		if(parentId!=-1){
			parent = bones[parentId];
			// start point
			v = parent->getAbsoluteTranslation();
			chMatrixInstance.transform(v);
			boneLines[lineNum].setDot1(v.x,v.y,v.z);
			// end point
			v = bone->getAbsoluteTranslation();
			chMatrixInstance.transform(v);
			boneLines[lineNum].setDot2(v.x,v.y,v.z);
			lineNum++;
		}
	}
	*num = lineNum;
	return boneLines;
}