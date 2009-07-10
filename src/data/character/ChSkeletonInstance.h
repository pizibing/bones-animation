#pragma once

class ChBoneInstance;
class ChSkeleton;
class ChAnimationManager;

// ChSkeletonInstance is a class that stores the current
// instance of a character's skeleton
// it is constructed by ChBoneInstances which match the
// ChBone stored in ChSkeleton
// this instance changes every frame via changing the ChBoneInstances
class ChSkeletonInstance
{
public:
	// constructor
	// this constructor will create all the ChBoneInstance corresponding
	// to ChBone in skeleton, store them in bones, and set the root.
	ChSkeletonInstance(ChSkeleton* skeleton);
	// destructor
	~ChSkeletonInstance(void);

	//get function of root
	ChBoneInstance* getRoot();

	// this function will calculate the current position of all the bone
	// instance of this skeleton.
	// animanager where you can get all animation
	// animation is the name of the animation to use, aimationtime is the 
	// current play time in animation
	void calSkeletonInstance(ChAnimationManager * animanager, float aimationtime, const char* animation);

	// this function will calculate the current position of all the bone
	// instance of this skeleton.
	// animanager where you can get all animation
	// animation1 and animation2 are the names of the animation to use,
	// this function will blend these two animations
	//  time_ms is the current play time in animation
	void calSkeletonInstance(ChAnimationManager * animanager, float aimationtime, const char* animation1, const char* animation2);

	// get a bone instance from the bones whose id equals to the given id
	ChBoneInstance* getBoneInstance(int id);

private:
	// calculate absolute transform for all bone instance 
	// with boneId as root bone
	void calculateAbsoluteTransform(int boneId);

private:
	// the root of this skeleton
	ChBoneInstance* root;
	// an array stores all the bones
	ChBoneInstance** bones;
	// the skeleton
	ChSkeleton* m_skeleton;
};
