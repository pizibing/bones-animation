#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
#include "ChKeyFrame.h"

// default constructor
ChKeyFrame::ChKeyFrame(void)
:m_time(0)
{}

// constructor
// @param matrix the transform matrix
ChKeyFrame::ChKeyFrame(const Matrix &matrix, float frame_time)
:m_transformMatrix(matrix),m_time(frame_time)
{
}

// destructor
ChKeyFrame::~ChKeyFrame(void){}

// @return the transform matrix
const Matrix & ChKeyFrame::getTransformMatrix()const{
	return m_transformMatrix;
}

// set the transform matrix
void ChKeyFrame::setTransformMatrix(const Matrix & matrix){
	m_transformMatrix = matrix;
}

// @return the start time(sec) of the frame
float ChKeyFrame::getTime(){
	return m_time;
}

// set the total time(sec) of the frame
inline void ChKeyFrame::setTime(float time){
	m_time = time;
}


