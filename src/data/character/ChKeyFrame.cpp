#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChKeyFrame.h"

// constructor
// @param matrix the transform matrix
ChKeyFrame::ChKeyFrame(const Matrix &matrix, int frame_time)
:/*m_transformMatrix(matrix),*/m_time(frame_time)
{
	setTransformMatrix(matrix);
}


// set the transform matrix
void ChKeyFrame::setTransformMatrix(const Matrix & matrix){
	//m_transformMatrix = matrix;
	m_rotation = matrix.getRotation();
	m_translation = matrix.getTranslation();
}
