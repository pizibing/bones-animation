#pragma once

#include <string>
#include <vector>
#include <map>

class Matrix;
class Quaternion;
class Vector3D;
class ChAnimation;
class ChSkeleton;


// ChAnimationManager manages all the animations and generate
// transform matrix for a bone
// ChAnimation has a table that contains all animations.

class ChAnimationManager
{
public:
	// constructor
	ChAnimationManager(void);
	// destructor
	~ChAnimationManager(void);

	// @return the skeleton to animate
	ChSkeleton * getSkeleton();
	
	// set the skeleton to animate
	void setSkeleton(ChSkeleton * skeleton);

	// @param name the name of the animation 
	// @return the pointer of animation with the name
	//         if no animation has the name, create a new animation
	ChAnimation * getAnimation(const std::string &name);

	// @param animationId the index of the animation 
	// @return the pointer of animation with the name
	//         if no animation has the index, create a new animation
	ChAnimation * getAnimation(int animationId);

	// no longer used
	// init the pointer array for animations
	// manager can be initialized only once
	// @param animation_num the number of animations
	// @return true if successful
	//         false if reinitialized or no memory
	bool init(int animation_num);

	// calculate the transform matrix for the bone with the animation
	// @param time_ms the time in second from the beginning of the
	//        animation
	// @param animation the name for the blending animation
	// @param bone the name for the blending bone
	// @return the transform matrix for the bone
	//Matrix blendAnimationBone(int animatetime,const std::string &animation,const std::string &bone);

	// calculate the rotate transform for the bone with the animation
	// @param time_ms the time in second from the beginning of the
	//        animation
	// @param animation the name for the blending animation
	// @param bone the name for the blending bone
	// @return the rotate transform for the bone
	Quaternion blendAnimationBoneRotation(int animatetime,const std::string &animation,const std::string &bone);

	// calculate the translate transform for the bone with the animation
	// @param time_ms the time in second from the beginning of the
	//        animation
	// @param animation the name for the blending animation
	// @param bone the name for the blending bone
	// @return the translate transform for the bone
	Vector3D blendAnimationBoneTranslation(int animatetime,const std::string &animation,const std::string &bone);

	// calculate the transform matrix for the bone with the animation
	// @param animatetime the time in second from the beginning of the
	//        animation
	// @param animationId the index for the blending animation
	// @param boneId the index for the blending bone
	// @return the transform matrix for the bone
	//Matrix blendAnimationBone(int animatetime,int animationId,int boneId);

	// calculate the rotate transform for the bone with the animation
	// @param animatetime the time in second from the beginning of the
	//        animation
	// @param animationId the index for the blending animation
	// @param boneId the index for the blending bone
	// @return the rotate transform for the bone
	Quaternion blendAnimationBoneRotation(int animatetime,int animationId,int boneId);

	// calculate the translate transform for the bone with the animation
	// @param animatetime the time in second from the beginning of the
	//        animation
	// @param animationId the index for the blending animation
	// @param boneId the index for the blending bone
	// @return the translate transform for the bone
	Vector3D blendAnimationBoneTranslation(int animatetime,int animationId,int boneId);

	// return the  root bone's transform matrix at animatetime
	// the result returned is from the second frame to the last frame
	// the first frame will not be returned for first frame equals last frame
	//Matrix getCurrentRootMatrix(int animatetime, const char* animation);

	// return the  root bone's rotate transform at animatetime
	// the result returned is from the second frame to the last frame
	// the first frame will not be returned for first frame equals last frame
	Quaternion getCurrentRootRotation(int animatetime, const char* animation);
	
	// return the  root bone's rotate transform at animatetime
	// the result returned is from the second frame to the last frame
	// the first frame will not be returned for first frame equals last frame
	Vector3D getCurrentRootTranslation(int animatetime, const char* animation);

	// return the root bone's transform matrix one frame before animatetime
	// the result returned is from the first frame to the last frame but one
	// the last frame will not be returned for first frame equals last frame
	//Matrix getLastRootMatrix(int animatetime, const char* animation);

	// return the root bone's rotate transform one frame before animatetime
	// the result returned is from the first frame to the last frame but one
	// the last frame will not be returned for first frame equals last frame
	Quaternion getLastRootRotation(int animatetime, const char* animation);

	// return the root bone's translate transform one frame before animatetime
	// the result returned is from the first frame to the last frame but one
	// the last frame will not be returned for first frame equals last frame
	Vector3D getLastRootTranslation(int animatetime, const char* animation);

private:
	// the skeleton to animate
	ChSkeleton * m_skeleton;
	// the number of animations
	//int m_animation_num;
	// pointer array for animations
	std::vector<ChAnimation*> m_animations;
	// map the name with the index of animation
	std::map< std::string, int > m_animationMap;

};
