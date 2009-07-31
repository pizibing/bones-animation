#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChTrack.h"
#include "ChBone.h"
#include "ChKeyFrame.h"

// constructor
ChTrack::ChTrack(void)
:m_keyframes(NULL),m_keyframe_num(0),m_current_keyframe(0)
{}

// destructor
ChTrack::~ChTrack(void){
	for(int i=0;i<m_keyframe_num;i++){
		delete m_keyframes[i];
	}
	if(m_keyframes)
		delete [] m_keyframes;
	m_keyframes = NULL;
}

//// @param animate_time
//// @return the relative transform matrix for the bone
//const Matrix& ChTrack::getTransformMatrix(int animate_time){
//	int i = 0;
//	// calculate the actual animation time
//	animate_time = animate_time % getAnimationTime();
//
//	while(animate_time>m_keyframes[i]->getTime()&&i<m_keyframe_num)i++;
//	if(i==m_keyframe_num)return m_keyframes[i-1]->getTransformMatrix();
//	ChKeyFrame *keyFrame1 = m_keyframes[i-1];
//	ChKeyFrame *keyFrame2 = m_keyframes[i];
//	float factor = ((float)animate_time - keyFrame1->getTime())/(keyFrame2->getTime()-keyFrame1->getTime());
//	m_currentMatrix = keyFrame1->getTransformMatrix()*(1-factor)+keyFrame2->getTransformMatrix()*factor;
//	return m_currentMatrix;
//}

// @param animate_time the frame time
// @return the relative rotate transform for the bone
const Quaternion & ChTrack::getRotation(int animate_time){
	if(m_keyframe_num ==0){
		m_currentRotation = m_bone->getRotation();
		return m_currentRotation;
	}
	// calculate the actual animation time
	animate_time = animate_time % getAnimationTime();
	int i = 0;
	while(animate_time>m_keyframes[i]->getTime()&&i<m_keyframe_num)i++;
	ChKeyFrame *keyFrame1;
	if(i>0) keyFrame1= m_keyframes[i-1];
	else keyFrame1= m_keyframes[m_keyframe_num-1];
	ChKeyFrame *keyFrame2 = m_keyframes[i];
	float factor = (float)(animate_time - keyFrame1->getTime())/float(keyFrame2->getTime()-keyFrame1->getTime());
	m_currentRotation = Quaternion::slerp(keyFrame1->getRotation(),keyFrame2->getRotation(),factor);
	return m_currentRotation;
}

// @param animate_time the frame time
// @return the relative translate transform for the bone
const Vector3D & ChTrack::getTranslation(int animate_time){
	if(m_keyframe_num ==0){
		 m_currentTranslation = m_bone->getTranslation();
		 return m_currentTranslation;
	}
	// calculate the actual animation time
	animate_time = animate_time % getAnimationTime();
	int i = 0;
	while(animate_time>m_keyframes[i]->getTime()&&i<m_keyframe_num)i++;
	ChKeyFrame *keyFrame1;
	if(i>0) keyFrame1= m_keyframes[i-1];
	else keyFrame1= m_keyframes[m_keyframe_num-1];
	ChKeyFrame *keyFrame2 = m_keyframes[i];
	float factor = (float)(animate_time - keyFrame1->getTime())/float(keyFrame2->getTime()-keyFrame1->getTime());
	m_currentTranslation = keyFrame1->getTranslation()*(1-factor)+keyFrame2->getTranslation()*factor;
	return m_currentTranslation;
}

//// @return the relative transform matrix of the last frame
//const Matrix& ChTrack::getLastTransformMatrix(){
//	return m_keyframes[m_keyframe_num - 1]->getTransformMatrix();
//}

// @return the relative rotate transform of the last frame
const Quaternion& ChTrack::getLastRotation() const{
	if(m_keyframe_num == 0 ) return m_bone->getRotation();
	return m_keyframes[m_keyframe_num - 1]->getRotation();
}

// @return the relative translate transform of the last frame
const Vector3D& ChTrack::getLastTranslation() const{
	if(m_keyframe_num == 0 ) return m_bone->getTranslation();
	return m_keyframes[m_keyframe_num - 1]->getTranslation(); 
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
// @param frame_time the start time(sec) of the frame
bool ChTrack::addKeyFrame(const Matrix &matrix,int frame_time){
	if(m_current_keyframe<m_keyframe_num){
		m_keyframes[m_current_keyframe]=new ChKeyFrame(matrix,frame_time);
		m_current_keyframe++;
	}
	return false;
}

// @return the total animation time of the track
int ChTrack::getAnimationTime() const{
	if(m_keyframe_num != 0)
		return m_keyframes[m_keyframe_num-1]->getTime();
	else return 0;
}

// initialized the key frame array
// @param frame_num the array size
// @return true if successful
//		   false if reinitialized or no more memory
bool ChTrack::init(int frame_num){
	if(m_keyframes)return false;
	m_keyframes = new ChKeyFrame*[frame_num];
	for(int i=0;i<m_keyframe_num;i++){
		m_keyframes[i] = NULL; 
	}
	m_keyframe_num = frame_num;
	return false;
}