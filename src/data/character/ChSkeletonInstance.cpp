#include "ChSkeletonInstance.h"

ChSkeletonInstance::ChSkeletonInstance(const ChSkeleton* skeleton){
}

ChSkeletonInstance::~ChSkeletonInstance(void){
}

//get function of root
ChBoneInstance* ChSkeletonInstance::getRoot(){
	return 0;
}

// this function will calculate the current position of all the bone
// instance of this skeleton.
// animation is the name of the animation to use, time_ms is the 
// current play time in animation
void ChSkeletonInstance::calSkeletonInstance(int time_ms, const char* animation){
}

// this function will calculate the current position of all the bone
// instance of this skeleton.
// animation1 and animation2 are the names of the animation to use,
// this function will blend these two animations
//  time_ms is the current play time in animation
void ChSkeletonInstance::calSkeletonInstance(int time_ms, const char* animation1, const char* animation2){}

// get a bone instance from the bones whose id equals to the given id
ChBoneInstance* ChSkeletonInstance::getBoneInstance(int id){
	return 0;
}