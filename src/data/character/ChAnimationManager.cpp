#include "../matrixlib/mtxlib.h"
#include "ChAnimationManager.h"
#include "ChAnimation.h"
#include "ChSkeleton.h"
//#include "../matrixlib/matrix.h"

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
	int id = m_animationMap[name];
	if(id>=0&&id<m_animation_num){
		return m_animations[id];
	}
	id = m_animationMap.size();
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
Matrix ChAnimationManager::blendAnimationBone(float animatetime,const std::string &animation,const std::string &bone){
	int animationId = m_animationMap[animation];
	int boneId = m_skeleton->getBoneId(bone);
	if(animationId>=0&&animationId<=m_animation_num){
		return m_animations[animationId]->blendBone(animatetime,boneId);
	}
	return Matrix();
}

// calculate the transform matrix for the bone with the animation
// @param animatetime the time in second from the beginning of the
//        animation
// @param animationId the index for the blending animation
// @param boneId the index for the blending bone
// @return the transform matrix for the bone
Matrix ChAnimationManager::blendAnimationBone(float animatetime,int animationId,int boneId){
	return m_animations[animationId]->blendBone(animatetime,boneId);
}
