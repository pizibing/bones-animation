#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
#include "ChAnimation.h"
#include "ChBone.h"
#include "ChSkeleton.h"
#include "ChTrack.h"

// constructor
ChAnimation::ChAnimation(void)
: m_tracks(NULL),m_key_frame_num(0)
{}

// destructor
ChAnimation::~ChAnimation(void){}

// @return the name for the animation
const std::string& ChAnimation::getName(){
	return m_name;
}

// set the name of the animation
void ChAnimation::setName(const std::string &name){
	m_name = name;
}

// set skeleton and init the animation
// and initialize track of bone
void ChAnimation::setSkeleton(ChSkeleton * skeleton){
	m_skeleton = skeleton;
	init(m_skeleton->getBoneNum());
}

// @return the track with the bonename
ChTrack * ChAnimation::getTrack(std::string bonename){
	int id = m_skeleton->getBoneId(bonename);
	if(id>=0&&id<m_track_num)return m_tracks[id];
	return NULL;
}

// @return the track with the boneId
ChTrack * ChAnimation::getTrack(int boneId){
	if(boneId>=0&&boneId<m_track_num)return m_tracks[boneId];
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
const Matrix& ChAnimation::blendBone(int time_ms,int boneId){
	return m_tracks[boneId]->getTransformMatrix(time_ms);
}

// @return the total time of the animation;
int ChAnimation::getAnimationTime(){
	return 0;
}