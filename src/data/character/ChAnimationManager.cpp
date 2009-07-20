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
:m_animations(NULL),m_animation_num(0)
{
}

// destructor
ChAnimationManager::~ChAnimationManager(void){
	for(int i=0;i<m_animation_num;i++){
		delete m_animations[i];
	}
	if(m_animations){
		delete [] m_animations;
	}
	m_animations = NULL;
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
	int id = m_animationMap.size();
	if(id<m_animation_num){
		m_animations[id] = new ChAnimation();
		m_animations[id]->setName(name);
		m_animations[id]->setSkeleton(m_skeleton);
		m_animationMap[name] = id;
		return m_animations[id];
	}
	return NULL;
}

// @param animationId the index of the animation 
// @return the pointer of animation with the name
//         if no animation has the index, create a new animation
ChAnimation * ChAnimationManager::getAnimation(int animationId){
	if(animationId<0||animationId>=m_animation_num)return NULL;
	if(m_animations[animationId])return m_animations[animationId];
	std::string name = "animation"+animationId;
	m_animations[animationId] = new ChAnimation();
	m_animations[animationId]->setName(name);
	m_animations[animationId]->setSkeleton(m_skeleton);
	m_animationMap[name] = animationId;
	return m_animations[animationId];
}

// init the pointer array for animations
// @param animation_max the number of animations
// @return true if successful 
//         false if reinitialized or no memory
bool ChAnimationManager::init(int animation_num){
	if(m_animations)return false;

	m_animations = new ChAnimation*[animation_num];
	if(m_animations){
		for(int i=0;i<animation_num;i++){
			m_animations[i] = NULL;
		}
		m_animation_num = animation_num;
	}

	return false;
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
	int animationId = m_animationMap[animation];
	int boneId = m_skeleton->getBoneId(bone);
	if(animationId>=0&&animationId<=m_animation_num){
		return m_animations[animationId]->blendBoneRotation(animatetime,boneId);
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
	int animationId = m_animationMap[animation];
	int boneId = m_skeleton->getBoneId(bone);
	if(animationId>=0&&animationId<=m_animation_num){
		return m_animations[animationId]->blendBoneTranslation(animatetime,boneId);
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
	return m_animations[animationId]->blendBoneRotation(animatetime,boneId);
}

// calculate the translate transform for the bone with the animation
// @param animatetime the time in second from the beginning of the
//        animation
// @param animationId the index for the blending animation
// @param boneId the index for the blending bone
// @return the translate transform for the bone
Vector3D ChAnimationManager::blendAnimationBoneTranslation(int animatetime,int animationId,int boneId){
	return m_animations[animationId]->blendBoneTranslation(animatetime,boneId);
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
	int remain = animatetime % m_animations[animationId]->getAnimationTime();
	// if remain is 0, then animatetime is the last frame's time
	if(remain == 0)
		return m_animations[animationId]->getTrack(0)->getLastRotation();
	// else blend bone as usual
	else
		return m_animations[animationId]->blendBoneRotation(animatetime,0);
}

// return the  root bone's rotate transform at animatetime
// the result returned is from the second frame to the last frame
// the first frame will not be returned for first frame equals last frame
Vector3D ChAnimationManager::getCurrentRootTranslation(int animatetime, const char* animation){
	// animatetime should not be smaller than 0
	assert(animatetime >= 0);

	int animationId = m_animationMap[animation];
	// check whether animatetime can be exactly divided by total animation time
	int remain = animatetime % m_animations[animationId]->getAnimationTime();
	// if remain is 0, then animatetime is the last frame's time
	if(remain == 0)
		return m_animations[animationId]->getTrack(0)->getLastTranslation();
	// else blend bone as usual
	else
		return m_animations[animationId]->blendBoneTranslation(animatetime,0);
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
	// to avoid -1, set animatetime to next loop's corresponding value
	if(animatetime == 0) animatetime = m_animations[animationId]->getAnimationTime();
	// return last frame, root bone's matrix
	return m_animations[animationId]->blendBoneRotation(animatetime - 1,0);
}

// return the root bone's translate transform one frame before animatetime
// the result returned is from the first frame to the last frame but one
// the last frame will not be returned for first frame equals last frame
Vector3D ChAnimationManager::getLastRootTranslation(int animatetime, const char* animation){
	// animatetime should not be smaller than 0
	assert(animatetime >= 0);

	int animationId = m_animationMap[animation];
	// to avoid -1, set animatetime to next loop's corresponding value
	if(animatetime == 0) animatetime = m_animations[animationId]->getAnimationTime();
	// return last frame, root bone's matrix
	return m_animations[animationId]->blendBoneTranslation(animatetime - 1,0);
}