#include "../matrixlib/matrix.h"
#include "ChKeyFrame.h"

// default constructor
ChKeyFrame::ChKeyFrame(void){}

// constructor
// @param matrix the transform matrix
ChKeyFrame::ChKeyFrame(const Matrix &matrix, int frame_time){}

// destructor
ChKeyFrame::~ChKeyFrame(void){}

// @return the transform matrix
inline const Matrix & ChKeyFrame::getTransformMatrix()const{
	return m_transformMatrix;
}

// set the transform matrix
inline void ChKeyFrame::setTransformMatrix(const Matrix & matrix){
	m_transformMatrix = matrix;
}

// @return the start time(ms) of the frame
inline int ChKeyFrame::getTime(){
	return m_time;
}

// set the total time(ms) of the frame
inline void ChKeyFrame::setTime(int time){
	m_time = time;
}


