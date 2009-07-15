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
// animanager where you can get all animation
// animation is the name of the animation to use, aimationtime is the 
// current play time in animation
void ChSkeletonInstance::calSkeletonInstance(ChAnimationManager * animanager, float aimationtime, const char* animation){
	for(int i=0;i<m_skeleton->getBoneNum();i++){
		bones[i]->setMatrix(animanager->blendAnimationBone(aimationtime,animation,m_skeleton->getBone(i)->getName()));
	}
	calculateAbsoluteTransform(m_skeleton->getRootBone()->getId());
}

// this function will calculate the current position of all the bone
// instance of this skeleton.
// animanager where you can get all animation
// animation1 and animation2 are the names of the animation to use,
// this function will blend these two animations
//  time_ms is the current play time in animation
void ChSkeletonInstance::calSkeletonInstance(ChAnimationManager * animanager, float aimationtime, const char* animation1, const char* animation2){
	for(int i=0;i<m_skeleton->getBoneNum();i++){
		matrix44 m1 = animanager->blendAnimationBone(aimationtime,animation1,m_skeleton->getBone(i)->getName());
		matrix44 m2 = animanager->blendAnimationBone(aimationtime,animation2,m_skeleton->getBone(i)->getName());
		bones[i]->setMatrix(m1*0.5+m2*0.5);
	}
	calculateAbsoluteTransform(m_skeleton->getRootBone()->getId());
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