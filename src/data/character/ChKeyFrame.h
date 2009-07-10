#pragma once

class Matrix;

// key frame for a bone animation
// contains a 4*4 matrix for bone relative transform
// and a 4*4 matrix for bone absolute transform

class ChKeyFrame
{
public:
	// default constructor
	ChKeyFrame(void);

	// constructor
	// @param matrix the transform matrix
	ChKeyFrame(const Matrix &matrix, int frame_time);

	// destructor
	~ChKeyFrame(void);

	// @return the transform matrix
	inline const Matrix & getTransformMatrix()const;

	// set the transform matrix
	inline void setTransformMatrix(const Matrix & matrix);

	// @return the start time(ms) of the frame
	inline int getTime();

	// set the start time(ms) of the frame
	inline void setTime(int time);

private:
	// relative transform matrix in parent bone space
	Matrix m_transformMatrix;

	// the start time(ms) of the frame
	int m_time;

};
