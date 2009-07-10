#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
#include "ChTrack.h"
#include "ChBone.h"
#include "ChKeyFrame.h"

ChTrack::ChTrack(void){}

ChTrack::~ChTrack(void){}

// @param animate_time
// @return the relative transform matrix for the bone
const Matrix& ChTrack::getTransformMatrix(int animate_time)const{
	return m_currentMatrix;
}

// @return the bone of the track
ChBone * ChTrack::getBone()const{
	return m_bone;
}

// set bone of the track
void ChTrack::setBone(ChBone * bone){
	m_bone = bone;
}

// add a new key frame
// @param matrix the transform matrix for the frame
// @param frame_time the start time(ms) of the frame
bool ChTrack::addKeyFrame(const Matrix &matrix,int frame_time){
	return false;
}

// initialized the key frame array
// @param frame_num the array size
// @return true if successful
bool ChTrack::init(int frame_num){
	return false;
}