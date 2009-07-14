#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
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

// @param animate_time
// @return the relative transform matrix for the bone
const Matrix& ChTrack::getTransformMatrix(float animate_time){
	int i = 0;
	while(animate_time>m_keyframes[i]->getTime()&&i<m_keyframe_num)i++;
	if(i==m_keyframe_num)return m_keyframes[i-1]->getTransformMatrix();
	ChKeyFrame *keyFrame1 = m_keyframes[i-1];
	ChKeyFrame *keyFrame2 = m_keyframes[i];
	float factor = (animate_time - keyFrame1->getTime())/(keyFrame2->getTime()-keyFrame1->getTime());
	m_currentMatrix = keyFrame1->getTransformMatrix()*(1-factor)+keyFrame2->getTransformMatrix()*factor;
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
// @param frame_time the start time(sec) of the frame
bool ChTrack::addKeyFrame(const Matrix &matrix,float frame_time){
	if(m_current_keyframe<m_keyframe_num){
		m_keyframes[m_current_keyframe]=new ChKeyFrame(matrix,frame_time);
		m_current_keyframe++;
	}
	return false;
}

// @return the total animation time of the track
float ChTrack::getAnimationTime(){
	return m_keyframes[m_keyframe_num-1]->getTime();
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