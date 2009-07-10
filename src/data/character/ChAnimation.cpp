#include "../matrixlib/matrix.h"
#include "ChAnimation.h"
#include "ChBone.h"
#include "ChSkeleton.h"
#include "ChTrack.h"

ChAnimation::ChAnimation(void){}

ChAnimation::~ChAnimation(void){}

// set skeleton and init the animation
// and initialize track of bone
void ChAnimation::setSkeleton(ChSkeleton * skeleton){
	m_skeleton = skeleton;
	init(m_skeleton->getBoneNum());
}

// @return the track with the bonename
ChTrack * ChAnimation::getTrack(std::string bonename){
	return NULL;
}

// @return the track with the boneId
ChTrack * ChAnimation::getTrack(int boneId){
	return NULL;
}
// initialize the track array
bool ChAnimation::init(int track_num){
	return false;
}

// calculate the transform matrix for the bone
// @param time_ms the time in million second from the beginning of the
//        animation
// @param boneId the index for the blending bone
// @return the transform matrix for the bone
Matrix ChAnimation::blendBone(int time_ms,int boneId){
	return Matrix();
}

// @return the total time of the animation;
int ChAnimation::getAnimationTime(){
	return 0;
}