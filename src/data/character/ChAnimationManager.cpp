#include "../matrixlib/mtxlib.h"
#include "ChAnimationManager.h"
#include "ChAnimation.h"
//#include "../matrixlib/matrix.h"

// constructor
ChAnimationManager::ChAnimationManager(void){
}

// destructor
ChAnimationManager::~ChAnimationManager(void){
}

// @param name the name of the animation 
// @return the pointer of animation with the name
//         if no animation has the name, create a new animation
ChAnimation * ChAnimationManager::getAnimation(const std::string &name){
	return NULL;
}

// @param animationId the index of the animation 
// @return the pointer of animation with the name
//         if no animation has the name, create a new animation
ChAnimation * ChAnimationManager::getAnimation(int animationId){
	return NULL;
}

// init the pointer array for animations
// @param animation_max the number of animations
// @return true if successful 
bool ChAnimationManager::init(int animation_max){
	return false;
}

// calculate the transform matrix for the bone with the animation
// @param time_ms the time in million second from the beginning of the
//        animation
// @param animation the name for the blending animation
// @param bone the name for the blending bone
// @return the transform matrix for the bone
Matrix ChAnimationManager::blendAnimationBone(int time_ms,const std::string &animation,const std::string &bone){
	return Matrix();
}

// calculate the transform matrix for the bone with the animation
// @param time_ms the time in million second from the beginning of the
//        animation
// @param animationId the index for the blending animation
// @param boneId the index for the blending bone
// @return the transform matrix for the bone
Matrix ChAnimationManager::blendAnimationBone(int time_ms,int animationId,int boneId){
	return Matrix();
}
