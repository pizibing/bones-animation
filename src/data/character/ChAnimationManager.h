#pragma once

#include <string>
#include <map>

class ChAnimation;
class Matrix;

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

	// @param name the name of the animation 
	// @return the pointer of animation with the name
	//         if no animation has the name, create a new animation
	ChAnimation * getAnimation(const std::string &name);

	// init the pointer array for animations
	// manager can be initialized only once
	// @param animation_num the number of animations
	// @return true if successful 
	bool init(int animation_num);

	// calculate the transform matrix for the bone with the animation
	// @param time_ms the time in million second from the beginning of the
	//        animation
	// @param animation the name for the blending animation
	// @param bone the name for the blending bone
	// @return the transform matrix for the bone
	Matrix blendAnimationBone(int time_ms,const std::string &animation,const std::string &bone);

private:
	// the number of animations
	int m_animation_num;
	// pointer array for animations
	ChAnimation ** m_animations;
	// map the name with pointer of animation
	std::map< std::string, ChAnimation* > m_animationMap;

};
