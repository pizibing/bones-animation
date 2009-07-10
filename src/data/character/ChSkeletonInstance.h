#pragma once

class ChBoneInstance;

// ChSkeletonInstance is a class that stores the current
// instance of a character's skeleton
// it is constructed by ChBoneInstances which match the
// ChBone stored in ChSkeleton
// this instance changes every frame via changing the ChBoneInstances
class ChSkeletonInstance
{
public:
	// constructor
	ChSkeletonInstance(void);
	// destructor
	~ChSkeletonInstance(void);
};
