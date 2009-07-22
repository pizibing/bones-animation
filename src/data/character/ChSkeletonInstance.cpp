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
	for(int i=0;i<skeleton->getBoneNum();i++){
		bones[i] = new ChBoneInstance(i);
		// set inverse matrix
		bones[i]->setInverse(m_skeleton->getBone(bones[i]->getId())->getBindPoseInverse());
	}
	root = bones[skeleton->getRootBone()->getId()];
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
	// set root bone's matrix to identity matrix
	bones[0]->setMatrix(Matrix());
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

	for(int i=1;i<m_skeleton->getBoneNum();i++){
		// set root bone's matrix to identity matrix
		bones[0]->setMatrix(Matrix());
		// for all bones except root bone, set matrix to relative matrix this animation time
		
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
	current.set(animanager->getCurrentRootRotation(animationtime,animation),
	 animanager->getCurrentRootTranslation(animationtime,animation));
	// get last transform matrix of root
	Matrix lastm = Matrix();
	lastm.set(animanager->getLastRootRotation(animationtime,animation),
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
	bones[boneId]->setRotation(bones[boneId]->getRotation() * bones[parent->getId()]->getRotation());
	bones[boneId]->setTranslation( bones[boneId]->getTranslation() + bones[parent->getId()]->getTranslation());
	if(parent){
		//bones[boneId]->setMatrix(bones[boneId]->getMatrix()*bones[parent->getId()]->getMatrix());
		bones[boneId]->setAbsoluteRotation(bones[boneId]->getRotation() * bones[parent->getId()]->getRotation());
		bones[boneId]->setAbsoluteTranslation( bones[boneId]->getTranslation() * bones[parent->getId()]->getAbsoluteRotation() 
									+ bones[parent->getId()]->getAbsoluteTranslation() );
		ChBone ** childbones = bone->getAllChildBones();
		for(int i=0;i<bone->getChildBoneNum();i++){
			calculateAbsoluteTransform(childbones[i]->getId());
		}
	}
	else {	
		// this is a root bone
		bones[boneId]->setAbsoluteRotation(bones[boneId]->getRotation());
		bones[boneId]->setAbsoluteTranslation(bones[boneId]->getTranslation());
	}
}

// get the lines for the skeleton
// modify num to get the line number
SimpleLine * ChSkeletonInstance::getBoneLines(int *num){
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
		// get b
		parentId = m_skeleton->getBone(i)->getParentId();
		if(parentId!=-1){
			parent = bones[parentId];
			// start point
			v = parent->getAbsoluteTranslation();
			boneLines[lineNum].setDot1(v.x,v.y,v.z);
			// end point
			v = bone->getAbsoluteTranslation();
			boneLines[lineNum].setDot2(v.x,v.y,v.z);
			lineNum++;
		}
	}
	*num = lineNum;
	return boneLines;
}