#include "Vector3D.h"
#include "quaternion.h"
#include "matrix.h"
#include <limits>

// @return a matrix for the result of multiplication of this and another
Matrix Matrix::operator * (const Matrix & matrix) const{
	Matrix mx;
	// row 0
	mx.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
	mx.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
	mx.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
	mx.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
	// row 1
	mx.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
	mx.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
	mx.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
	mx.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
	// row 2
	mx.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
	mx.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
	mx.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
	mx.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
	// row 3
	mx.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
	mx.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
	mx.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
	mx.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
	return mx;
}

// multiply this matrix by another
inline void Matrix::operator *= (const Matrix & matrix){
	*this = *this * matrix;
}

// @return a matrix  for the result of this matrix scaled by factor
inline Matrix Matrix::operator * (float factor) const{
	Matrix matrix (*this);
	for(int i=0;i<16;i++)
		matrix[i]*=factor;
	return matrix;
}

// scale this matrix by factor
inline void Matrix::operator *= (float factor){
	for(int i=0;i<16;i++) m[i]*=factor;
}

// set the matrix with the given rotation and translation
inline void Matrix::set(const Quaternion rotation, Vector3D translation){
	rotation.SetToMatrix(*this);
}

// set the matrix with the given rotation ,translation will not be changed
inline void Matrix::set(const Quaternion rotation){
	rotation.SetToMatrix(*this);
}

// set the matrix with the given translation, rotation will not be changed
inline void Matrix::set(Vector3D translation){
	// row 3
	m[3] = translation.x;
	m[7] = translation.y;
	m[11] = translation.z;
	m[15] = 1.0f;
}

// @return the rotation of the matrix
inline Quaternion Matrix::getRotation() const{
	return Quaternion::MatrixRotationQuaternion(*this);
}

// @return the translation of the matrix
inline Vector3D Matrix::getTranslation() const{
	return Vector3D(m[3],m[7],m[11]);
}

// Code taken and adapted from nVidia's nv_algebra: det2x2, det3x3, invert, multiply
// -----
// Calculate the determinant of a 2x2 matrix 
// |a1 a2|
// |b1 b2|
static float det2x2(float a1, float a2, float b1, float b2)
{
	return a1 * b2 - b1 * a2;
}

// Calculate the determinant of a 3x3 matrix
// |a1 a2 a3|
// |b1 b2 b3|
// |c1 c2 c3|
static float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
	return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}

// @return the inverse matrix of the matrix
inline Matrix Matrix::getInverseMatrix() const{
	Matrix b;

	b.m[0] =  det3x3(m[5], m[9], m[13], m[6], m[10], m[14], m[7], m[11], m[15]);
	b.m[4] = -det3x3(m[4], m[8], m[12], m[6], m[10], m[14], m[7], m[11], m[15]);
	b.m[8] =  det3x3(m[4], m[8], m[12], m[5], m[9], m[13], m[7], m[11], m[15]);
	b.m[12] = -det3x3(m[4], m[8], m[12], m[5], m[9], m[13], m[6], m[10], m[14]);

	b.m[1] = -det3x3(m[1], m[9], m[13], m[2], m[10], m[14], m[3], m[11], m[15]);
	b.m[5] =  det3x3(m[0], m[8], m[12], m[2], m[10], m[14], m[3], m[11], m[15]);
	b.m[9] = -det3x3(m[0], m[8], m[12], m[1], m[9], m[13], m[3], m[11], m[15]);
	b.m[13] =  det3x3(m[0], m[8], m[12], m[1], m[9], m[13], m[2], m[10], m[14]);

	b.m[2] =  det3x3(m[1], m[5], m[13], m[2], m[6], m[14], m[3], m[7], m[15]);
	b.m[6] = -det3x3(m[0], m[4], m[12], m[2], m[6], m[14], m[3], m[7], m[15]);
	b.m[10] =  det3x3(m[0], m[4], m[12], m[1], m[5], m[13], m[3], m[7], m[15]);
	b.m[14] = -det3x3(m[0], m[4], m[12], m[1], m[5], m[13], m[2], m[6], m[14]);

	b.m[3] = -det3x3(m[1], m[5], m[9], m[2], m[6], m[10], m[3], m[7], m[11]);
	b.m[7] =  det3x3(m[0], m[4], m[8], m[2], m[6], m[10], m[3], m[7], m[11]);
	b.m[11] = -det3x3(m[0], m[4], m[8], m[1], m[5], m[9], m[3], m[7], m[11]);
	b.m[15] =  det3x3(m[0], m[4], m[8], m[1], m[5], m[9], m[2], m[6], m[10]);

	double det = (m[0] * b.m[0]) + (m[1] * b.m[4]) + (m[2] * b.m[8]) + (m[3] * b.m[12]);

	double epsilon = std::numeric_limits<double>::epsilon();
	if (det + epsilon >= 0.0f && det - epsilon <= 0.0f) {
		det = 0.0001f;
		if(det<0)det *=-1;
	}
	float oodet = (float) (1.0 / det);

	b.m[0] *= oodet;
	b.m[4] *= oodet;
	b.m[8] *= oodet;
	b.m[12] *= oodet;

	b.m[1] *= oodet;
	b.m[5] *= oodet;
	b.m[9] *= oodet;
	b.m[13] *= oodet;

	b.m[2] *= oodet;
	b.m[6] *= oodet;
	b.m[10] *= oodet;
	b.m[14] *= oodet;

	b.m[3] *= oodet;
	b.m[7] *= oodet;
	b.m[11] *= oodet;
	b.m[15] *= oodet;

	return b;
}


// transform the position by the matrix
// (x,y,z,1) = (x,y,z,1)*[Matrix44]
// @param position float array for a vector3d
inline void Matrix::transform(float *position) const{
	float x,y,z;
	x = m[0] * position[0] + m[1] * position[1] + m[2] * position[2];
	y =	m[4] * position[0] + m[5] * position[1] + m[6] * position[2];
	z = m[8] * position[0] + m[9] * position[1] + m[10] * position[2];
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

// get vector transformed by the matrix
//  (x,y,z,1)*[Matrix44]
// @param v a vector3d to be transformed
// @return a vector transformed by the matrix
inline Vector3D Matrix::TransformVector(const Vector3D& v) const
{
	return Vector3D(
		m[0] * v.x + m[1] * v.y + m[2] * v.z,
		m[4] * v.x + m[5] * v.y + m[6] * v.z,
		m[8] * v.x + m[9] * v.y + m[10] * v.z
		);
}

// multiply a float by a matrix
inline Matrix operator * (float factor, const Matrix & matrix){
	return matrix*factor;
}