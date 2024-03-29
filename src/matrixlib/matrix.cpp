#include "Vector3D.h"
#include "quaternion.h"
#include "matrix.h"

#ifndef DBL_EPSILON
#define DBL_EPSILON     2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif

// add operator
Matrix Matrix::operator + (const Matrix & matrix) const{
	Matrix mx;
	for(int i = 0; i < 16; i++)
		mx.m[i] = m[i] + matrix[i];
	return mx;
}

// @return a matrix for the result of multiplication of this and another
Matrix Matrix::operator * (const Matrix & matrix) const{
	Matrix mx;
	mx.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
	mx.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
	mx.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
	mx.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
	mx.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
	mx.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
	mx.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
	mx.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
	mx.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
	mx.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
	mx.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
	mx.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
	mx.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
	mx.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
	mx.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
	mx.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
	return mx;
}

// multiply this matrix by another
void Matrix::operator *= (const Matrix & matrix){
	*this = *this * matrix;
}

Vector3D Matrix::operator * (const Vector3D &v) const{
	return Vector3D(
		m[0] * v.x + m[4] * v.y + m[8] * v.z,
		m[1] * v.x + m[5] * v.y + m[9] * v.z,
		m[2] * v.x + m[6] * v.y + m[10] * v.z
		);
}

// @return a matrix  for the result of this matrix scaled by factor
Matrix Matrix::operator * (float factor) const{
	Matrix matrix (*this);
	for(int i=0;i<16;i++)
		matrix[i]*=factor;
	return matrix;
}

// scale this matrix by factor
void Matrix::operator *= (float factor){
	for(int i=0;i<16;i++) m[i]*=factor;
}

// set the matrix with the given rotation and translation
void Matrix::set(const Quaternion rotation, Vector3D translation){
	rotation.SetToMatrix(*this);
	// row 3
	m[12] = translation.x;
	m[13] = translation.y;
	m[14] = translation.z;
	m[15] = 1.0f;
}

// set the matrix with the given rotation ,translation will not be changed
void Matrix::set(const Quaternion rotation){
	rotation.SetToMatrix(*this);
}

// set the matrix with the given translation, rotation will not be changed
void Matrix::set(Vector3D translation){
	// row 3
	m[12] = translation.x;
	m[13] = translation.y;
	m[14] = translation.z;
	m[15] = 1.0f;
}

// @return the rotation of the matrix
Quaternion Matrix::getRotation() const{
	return Quaternion::MatrixRotationQuaternion(*this);
}

// @return the translation of the matrix
Vector3D Matrix::getTranslation() const{
	return Vector3D(m[12],m[13],m[14]);
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
Matrix Matrix::getInverseMatrix() const{
	Matrix b;

	b.m[0] =  det3x3(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]);
	b.m[1] = -det3x3(m[1], m[2], m[3], m[9], m[10], m[11], m[13], m[14], m[15]);
	b.m[2] =  det3x3(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]);
	b.m[3] = -det3x3(m[1], m[2], m[3], m[5], m[6], m[7], m[9], m[10], m[11]);

	b.m[4] = -det3x3(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]);
	b.m[5] =  det3x3(m[0], m[2], m[3], m[8], m[10], m[11], m[12], m[14], m[15]);
	b.m[6] = -det3x3(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]);
	b.m[7] =  det3x3(m[0], m[2], m[3], m[4], m[6], m[7], m[8], m[10], m[11]);

	b.m[8] =  det3x3(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]);
	b.m[9] = -det3x3(m[0], m[1], m[3], m[8], m[9], m[11], m[12], m[13], m[15]);
	b.m[10] =  det3x3(m[0], m[1], m[3], m[4], m[5], m[7], m[12], m[13], m[15]);
	b.m[11] = -det3x3(m[0], m[1], m[3], m[4], m[5], m[7], m[8], m[9], m[11]);

	b.m[12] = -det3x3(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);
	b.m[13] =  det3x3(m[0], m[1], m[2], m[8], m[9], m[10], m[12], m[13], m[14]);
	b.m[14] = -det3x3(m[0], m[1], m[2], m[4], m[5], m[6], m[12], m[13], m[14]);
	b.m[15] =  det3x3(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);

	double det = (m[0] * b.m[0]) + (m[4] * b.m[1]) + (m[8] * b.m[2]) + (m[12] * b.m[3]);

	double epsilon = DBL_EPSILON;
	if (det + epsilon >= 0.0f && det - epsilon <= 0.0f) {
		det = 0.0001f;
		if(det<0)det *=-1;
	}
	float oodet = (float) (1.0 / det);

	b.m[0] *= oodet;
	b.m[1] *= oodet;
	b.m[2] *= oodet;
	b.m[3] *= oodet;

	b.m[4] *= oodet;
	b.m[5] *= oodet;
	b.m[6] *= oodet;
	b.m[7] *= oodet;

	b.m[8] *= oodet;
	b.m[9] *= oodet;
	b.m[10] *= oodet;
	b.m[11] *= oodet;

	b.m[12] *= oodet;
	b.m[13] *= oodet;
	b.m[14] *= oodet;
	b.m[15] *= oodet;

	return b;
}


// transform the position by the matrix
// (x,y,z) = (x,y,z)* rotation + translation
// @param position float array for a vector3d
void Matrix::transform(float *position) const{
	float x = position[0];
	float y = position[1];
	float z = position[2];
	position[0] = m[0] * x + m[4] * y + m[8] * z + m[12];
	position[1] = m[1] * x + m[5] * y + m[9] * z + m[13];
	position[2] = m[2] * x + m[6] * y + m[10] * z + m[14];
}

void Matrix::transform(Vector3D & position) const{
	float x = position.x;
	float y = position.y;
	float z = position.z;
	position.x = m[0] * x + m[4] * y + m[8] * z + m[12];
	position.y = m[1] * x + m[5] * y + m[9] * z + m[13];
	position.z = m[2] * x + m[6] * y + m[10] * z + m[14];
}

// get vector transformed by the matrix
//  (x,y,z) = (x,y,z) * rotation
// @param v a vector3d to be transformed
// @return a vector transformed by the matrix
void Matrix::transformVector(float * v) const
{
	float x = v[0];
	float y = v[1];
	float z = v[2];	
	v[0] = m[0] * x + m[4] * y + m[8] * z;
	v[1] = m[1] * x + m[5] * y + m[9] * z;
	v[2] = m[2] * x + m[6] * y + m[10] * z;
}

// get vector transformed by the matrix
//  (x,y,z) = (x,y,z) * rotation
// @param v a vector3d to be transformed
// @return a vector transformed by the matrix
void Matrix::transformVector(Vector3D & v) const
{
	float x = v.x;
	float y = v.y;
	float z = v.z;	
	v.x = m[0] * x + m[4] * y + m[8] * z;
	v.y = m[1] * x + m[5] * y + m[9] * z;
	v.z = m[2] * x + m[6] * y + m[10] * z;
}

// the same as transform
Vector3D operator * (const Vector3D& v, const Matrix& m){
	Vector3D nv;
	m.transform(nv);
	return nv;
}

// multiply a float by a matrix
Matrix operator * (float factor, const Matrix & matrix){
	return matrix*factor;
}

// rotate the matrix with axis (x,y,z) by angle degree
void Matrix::Rotate(float x, float y, float z, float angle){
	(*this)*=AxisRotationMatrix(x,y,z,angle/180.0f*M_PI);
}

// rotate the matrix by quaternion
void Matrix::Rotate(const Quaternion & q){
	Vector3D axis;
	float angle;
	q.ToAngleAxis(axis,angle);
	(*this)*=AxisRotationMatrix(axis.x,axis.y,axis.z,angle);
}

// rotate the matrix with by angle vector (x,y,z)
void Matrix::Translate(float x, float y, float z){
	m[12] += x;
	m[13] += y;
	m[14] += z;
}

// rotate the matrix with by angle vector3d
void Matrix::Translate(const Vector3D & v){
	m[12] += v.x;
	m[13] += v.y;
	m[14] += v.z;
}

// get rotation Matrix with axis(x,y,z) by angle in radians
Matrix Matrix::AxisRotationMatrix(float x, float y, float z, float angle)
{
	Matrix matrix;
	Vector3D a(x,y,z);
	a.Unitize();
	float xSq = a.x * a.x;
	float ySq = a.y * a.y;
	float zSq = a.z * a.z;
	float cT = cosf(angle);
	float sT = sinf(angle);

	matrix[0] = xSq + (ySq + zSq) * cT;
	matrix[1] = a.x * a.y * (1.0f - cT) + a.z * sT;
	matrix[2] = a.x * a.z * (1.0f - cT) - a.y * sT;
	matrix[3] = 0;
	matrix[4] = a.x * a.y * (1.0f - cT) - a.z * sT;
	matrix[5] = ySq + (xSq + zSq) * cT;
	matrix[6] = a.y * a.z * (1.0f - cT) + a.x * sT;
	matrix[7] = 0;
	matrix[8] = a.x * a.z * (1.0f - cT) + a.y * sT;
	matrix[9] = a.y * a.z * (1.0f - cT) - a.x * sT;
	matrix[10] = zSq + (xSq + ySq) * cT;
	matrix[11] = 0;
	matrix[14] = matrix[13] = matrix[12] = 0;
	matrix[15] = 1;

	return matrix;
}