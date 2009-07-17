#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChAnimation.h"
#include "ChBone.h"
#include "ChSkeleton.h"
#include "ChTrack.h"

// constructor
ChAnimation::ChAnimation(void)
: m_tracks(NULL),m_key_frame_num(0)
{}

// destructor
ChAnimation::~ChAnimation(void){
	for(int i=0;i<m_track_num;i++){
		delete m_tracks[i];
	}
	if(m_tracks)
		delete [] m_tracks;
	m_tracks = NULL;
}

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
// @return true if successful
//         false if reinitialized or no more memory
bool ChAnimation::init(int track_num){
	if(m_tracks)return false;

	m_tracks = new ChTrack*[track_num];
	if(m_tracks){
		for(int i=0;i<track_num;i++){
			m_tracks[i] = new ChTrack();
			m_tracks[i]->setBone(m_skeleton->getBone(i));
		}
		m_track_num = track_num;
		return true;
	}
	return false;
}

// calculate the transform matrix for the bone
// @param animatetime the time in second from the beginning of the
//        animation
// @param boneId the index for the blending bone
// @return the transform matrix for the bone
//const Matrix& ChAnimation::blendBone(int animatetime,int boneId){
//	return m_tracks[boneId]->getTransformMatrix(animatetime);
//}

// calculate the rotate transform for the bone
// @param animatetime the time in second from the beginning of the
//        animation
// @param boneId the index for the blending bone
// @return the rotate transform for the bone
const Quaternion& ChAnimation::blendBoneRotation(int animatetime,int boneId){
	return m_tracks[boneId]->getRotation(animatetime);
}

// calculate the translate transform for the bone
// @param animatetime the time in second from the beginning of the
//        animation
// @param boneId the index for the blending bone
// @return the translate transform for the bone
const Vector3D& ChAnimation::blendBoneTranslation(int animatetime,int boneId){
	return m_tracks[boneId]->getTranslation(animatetime);
}

// @return the total time of the animation;
int ChAnimation::getAnimationTime(){
	return m_tracks[0]->getAnimationTime();
}