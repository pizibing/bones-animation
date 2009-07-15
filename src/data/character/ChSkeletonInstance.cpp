#include "../matrixlib/mtxlib.h"
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
matrix44 ChSkeletonInstance::calSkeletonInstance(ChAnimationManager * animanager, int aimationtime, const char* animation){
	// set root bone's matrix to identity matrix
	bones[0]->setMatrix(matrix44());
	// for all bones except root bone, set matrix to relative matrix this animation time
	for(int i=1;i<m_skeleton->getBoneNum();i++){
		bones[i]->setMatrix(animanager->blendAnimationBone(aimationtime,animation,m_skeleton->getBone(i)->getName()));
	}
	calculateAbsoluteTransform(m_skeleton->getRootBone()->getId());

	// use getCurrentRootMatrix and getLastRootMatrix to calculate
	// the return matrix44
	return matrix44();
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
matrix44 ChSkeletonInstance::calSkeletonInstance(ChAnimationManager * animanager, int animationtime1,
												 int animationtime2, const char* animation1, const char* animation2
												 , float power1){
	// power1 should be 0 to 1
    assert(power1 > 0);
	assert(power1 < 1);

	for(int i=1;i<m_skeleton->getBoneNum();i++){
		// set root bone's matrix to identity matrix
		bones[0]->setMatrix(matrix44());
		// for all bones except root bone, set matrix to relative matrix this animation time
		matrix44 m1 = animanager->blendAnimationBone(animationtime1,animation1,m_skeleton->getBone(i)->getName());
		matrix44 m2 = animanager->blendAnimationBone(animationtime2,animation2,m_skeleton->getBone(i)->getName());
		bones[i]->setMatrix(m1*power1+m2*(1-power1));
	}
	calculateAbsoluteTransform(m_skeleton->getRootBone()->getId());

	// use getCurrentRootMatrix and getLastRootMatrix to calculate
	// the return matrix44
	return matrix44();
}

// get a bone instance from the bones whose id equals to the given id
ChBoneInstance* ChSkeletonInstance::getBoneInstance(int id){
	assert(id>=0&&id<m_skeleton->getBoneNum());
	return bones[id];
}

// calculate absolute transform for all bone instance 
// with boneId as root bone
void ChSkeletonInstance::calculateAbsoluteTransform(int boneId){
	ChBone * bone = m_skeleton->getBone(boneId);
	ChBone * parent = bone->getParentBone();

	if(parent){
		bones[boneId]->setMatrix(bones[boneId]->getMatrix()*bones[parent->getId()]->getMatrix());
		ChBone ** childbones = bone->getAllChildBones();
		for(int i=0;i<bone->getChildBoneNum();i++){
			calculateAbsoluteTransform(childbones[i]->getId());
		}
	}
	else {
		// this is a root bone
	}
}