#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChTrack.h"
#include "ChSkeleton.h"
#include "ChAnimation.h"
#include "ChAnimationManager.h"

#include <assert.h>

// constructor
ChAnimationManager::ChAnimationManager(void)
{
}

// destructor
ChAnimationManager::~ChAnimationManager(void){
	for(unsigned int i=0;i<m_animations.size();i++){
		delete m_animations[i];
		m_animations[i] = NULL;
	}
	m_animations.clear();
}

// @return the skeleton to animate
ChSkeleton * ChAnimationManager::getSkeleton(){
	return m_skeleton;
}

// set the skeleton to animate
void ChAnimationManager::setSkeleton(ChSkeleton * skeleton){
	m_skeleton = skeleton;
}

// @param name the name of the animation 
// @return the pointer of animation with the name
//         if no animation has the name, create a new animation
ChAnimation * ChAnimationManager::getAnimation(const std::string &name){
	std::map<std::string,int>::iterator i = m_animationMap.find(name);
	if(i!=m_animationMap.end()){
		return m_animations[i->second];
	}
	int id = m_animations.size();
	
	ChAnimation *animation = new ChAnimation();
	
	animation->setName(name);
	animation->setSkeleton(m_skeleton);
	m_animationMap[name] = id;
	m_animations.push_back(animation);
	return m_animations[id];
}

// @param animationId the index of the animation 
// @return the pointer of animation with the name
//         if no animation has the index, create a new animation
ChAnimation * ChAnimationManager::getAnimation(int animationId){
	unsigned int index = (unsigned int)animationId;
	if(index<m_animations.size())
		return m_animations[index];
	else return NULL;
}

// init the pointer array for animations
// @param animation_max the number of animations
// @return true if successful 
//         false if reinitialized or no memory
bool ChAnimationManager::init(int animation_num){
	unsigned int num = (unsigned int)animation_num;
	if(m_animations.size() >= num)return false;
	m_animations.reserve(num);
	return true;
}

// calculate the transform matrix for the bone with the animation
// @param animatetime the time in second from the beginning of the
//        animation
// @param animation the name for the blending animation
// @param bone the name for the blending bone
// @return the transform matrix for the bone
//Matrix ChAnimationManager::blendAnimationBone(int animatetime,const std::string &animation,const std::string &bone){
//	int animationId = m_animationMap[animation];
//	int boneId = m_skeleton->getBoneId(bone);
//	if(animationId>=0&&animationId<=m_animation_num){
//		return m_animations[animationId]->blendBone(animatetime,boneId);
//	}
//	return Matrix();
//}

// calculate the rotate transform for the bone with the animation
// @param time_ms the time in second from the beginning of the
//        animation
// @param animation the name for the blending animation
// @param bone the name for the blending bone
// @return the rotate transform for the bone
Quaternion ChAnimationManager::blendAnimationBoneRotation(int animatetime,const std::string &animation,const std::string &bone){
	std::map<std::string,int>::iterator i = m_animationMap.find(animation);
	if(i!=m_animationMap.end()){
		unsigned int animationId = i->second;
		int boneId = m_skeleton->getBoneId(bone);
		ChAnimation * animation = m_animations[animationId];
		return animation->blendBoneRotation(animatetime,boneId);
	}
		return Quaternion();
}

// calculate the translate transform for the bone with the animation
// @param time_ms the time in second from the beginning of the
//        animation
// @param animation the name for the blending animation
// @param bone the name for the blending bone
// @return the translate transform for the bone
Vector3D ChAnimationManager::blendAnimationBoneTranslation(int animatetime,const std::string &animation,const std::string &bone){
	std::map<std::string,int>::iterator i = m_animationMap.find(animation);
	if(i!=m_animationMap.end()){
		unsigned int animationId = i->second;
		int boneId = m_skeleton->getBoneId(bone);
		ChAnimation * animation = m_animations[animationId];
		return animation->blendBoneTranslation(animatetime,boneId);
	}
	return Vector3D();
}

// calculate the transform matrix for the bone with the animation
// @param animatetime the time in second from the beginning of the
//        animation
// @param animationId the index for the blending animation
// @param boneId the index for the blending bone
// @return the transform matrix for the bone
//Matrix ChAnimationManager::blendAnimationBone(int animatetime,int animationId,int boneId){
//	return m_animations[animationId]->blendBone(animatetime,boneId);
//}

// calculate the rotate transform for the bone with the animation
// @param animatetime the time in second from the beginning of the
//        animation
// @param animationId the index for the blending animation
// @param boneId the index for the blending bone
// @return the rotate transform for the bone
Quaternion ChAnimationManager::blendAnimationBoneRotation(int animatetime,int animationId,int boneId){
	ChAnimation * animation = m_animations[animationId];
	return animation->blendBoneRotation(animatetime,boneId);
}

// calculate the translate transform for the bone with the animation
// @param animatetime the time in second from the beginning of the
//        animation
// @param animationId the index for the blending animation
// @param boneId the index for the blending bone
// @return the translate transform for the bone
Vector3D ChAnimationManager::blendAnimationBoneTranslation(int animatetime,int animationId,int boneId){
	ChAnimation * animation = m_animations[animationId];
	return animation->blendBoneTranslation(animatetime,boneId);
}

// return the  root bone's transform matrix at animatetime
// the result returned is from the second frame to the last frame
// the first frame will not be returned for first frame equals last frame
//Matrix ChAnimationManager::getCurrentRootMatrix(int animatetime, const char* animation){
//
//	// animatetime should not be smaller than 0
//	assert(animatetime >= 0);
//	
//	int animationId = m_animationMap[animation];
//	// check whether animatetime can be exactly divided by total animation time
//	int remain = animatetime % m_animations[animationId]->getAnimationTime();
//	// if remain is 0, then animatetime is the last frame's time
//	if(remain == 0)
//		return m_animations[animationId]->getTrack(0)->getLastTransformMatrix();
//	// else blend bone as usual
//	else
//		return m_animations[animationId]->blendBone(animatetime,0);
//	
//}

// return the  root bone's rotate transform at animatetime
// the result returned is from the second frame to the last frame
// the first frame will not be returned for first frame equals last frame
Quaternion ChAnimationManager::getCurrentRootRotation(int animatetime, const char* animation){
	// animatetime should not be smaller than 0
	assert(animatetime >= 0);

	int animationId = m_animationMap[animation];
	// check whether animatetime can be exactly divided by total animation time
	ChAnimation * chanimation = m_animations[animationId];
	int remain = animatetime % chanimation->getAnimationTime();
	// if remain is 0, then animatetime is the last frame's time
	if(remain == 0)
		return chanimation->getTrack(0)->getLastRotation();
	// else blend bone as usual
	else
		return chanimation->blendBoneRotation(animatetime,0);
}

// return the  root bone's rotate transform at animatetime
// the result returned is from the second frame to the last frame
// the first frame will not be returned for first frame equals last frame
Vector3D ChAnimationManager::getCurrentRootTranslation(int animatetime, const char* animation){
	// animatetime should not be smaller than 0
	assert(animatetime >= 0);

	int animationId = m_animationMap[animation];
	// check whether animatetime can be exactly divided by total animation time
	ChAnimation * chanimation = m_animations[animationId];
	int remain = animatetime % chanimation->getAnimationTime();
	// if remain is 0, then animatetime is the last frame's time
	if(remain == 0)
		return chanimation->getTrack(0)->getLastTranslation();
	// else blend bone as usual
	else
		return chanimation->blendBoneTranslation(animatetime,0);
}

// return the root bone's transform matrix one frame before animatetime
// the result returned is from the first frame to the last frame but one
// the last frame will not be returned for first frame equals last frame
//Matrix ChAnimationManager::getLastRootMatrix(int animatetime, const char* animation){
//
//	// animatetime should not be smaller than 0
//	assert(animatetime >= 0);
//
//	int animationId = m_animationMap[animation];
//	// to avoid -1, set animatetime to next loop's corresponding value
//	if(animatetime == 0) animatetime = m_animations[animationId]->getAnimationTime();
//	// return last frame, root bone's matrix
//	return m_animations[animationId]->blendBone(animatetime - 1,0);
//}

// return the root bone's rotate transform one frame before animatetime
// the result returned is from the first frame to the last frame but one
// the last frame will not be returned for first frame equals last frame
Quaternion ChAnimationManager::getLastRootRotation(int animatetime, const char* animation){
	// animatetime should not be smaller than 0
	assert(animatetime >= 0);

	int animationId = m_animationMap[animation];
	ChAnimation * chanimation = m_animations[animationId];
	// to avoid -1, set animatetime to next loop's corresponding value
	if(animatetime == 0) animatetime = chanimation->getAnimationTime();
	// return last frame, root bone's matrix
	return chanimation->blendBoneRotation(animatetime - 1,0);
}

// return the root bone's translate transform one frame before animatetime
// the result returned is from the first frame to the last frame but one
// the last frame will not be returned for first frame equals last frame
Vector3D ChAnimationManager::getLastRootTranslation(int animatetime, const char* animation){
	// animatetime should not be smaller than 0
	assert(animatetime >= 0);

	int animationId = m_animationMap[animation];
	ChAnimation * chanimation = m_animations[animationId];
	// to avoid -1, set animatetime to next loop's corresponding value
	// to avoid -1, set animatetime to next loop's corresponding value
	if(animatetime == 0) animatetime = chanimation->getAnimationTime();
	// return last frame, root bone's matrix
	return chanimation->blendBoneTranslation(animatetime - 1,0);
}