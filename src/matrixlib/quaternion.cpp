#include "quaternion.h"
#include "matrix.h"
#include "Vector3D.h"

// r = this * q
// @return a quaternion for the multiplication of this and q
Quaternion Quaternion::operator*(const Quaternion& q) const{
	Quaternion r;
	/*r.x = q.w * x + q.x * w + q.y * z - q.z * y;
	r.y = q.w * y + q.y * w + q.z * x - q.x * z;
	r.z = q.w * z + q.z * w + q.x * y - q.y * x;
	r.w = q.w * w - q.x * x - q.y * y - q.z * z;*/
	r.x =  x * q.w + y * q.z - z * q.y + w * q.x;
	r.y = -x * q.z + y * q.w + z * q.x + w * q.y;
	r.z =  x * q.y - y * q.x + z * q.w + w * q.z;
	r.w = -x * q.x - y * q.y - z * q.z + w * q.w;
	return r;
}

// rotate the vector3d by this quaternion
Vector3D Quaternion::operator*(const Vector3D& v) const{
	Quaternion out = (*this) * Quaternion(v.x, v.y, v.z, 0.0f) * (~(*this));
	return Vector3D(out.x, out.y, out.z);
}

// rotate the vector3d by this quaternion
Vector3D operator*(const Vector3D& v, const Quaternion& q) {
	Quaternion out = q * Quaternion(v.x, v.y, v.z, 0.0f) * (~q);
	return Vector3D(out.x, out.y, out.z);
}

/** Returns the slerp of this quaternion to other at factor time
@param other The Quaternion to interpolate to
@param factor The percentage (0 < time < 1) to slerp
@return The Quaternion formed by the slerp */
Quaternion Quaternion::slerp(const Quaternion& other, float factor) const{
	//quaternion must be normalized before operation
	Quaternion q1 = this->Normalize();
	Quaternion q2 = other.Normalize();

	if(factor>=1.0f) return q2;
	if(factor<=0.0f) return q1;
	float theta, st, sut, sout, interp1, interp2;
	float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z +
		q1.w * q2.w;

	if (dot >= 1.0f) return q1;

	// algorithm taken from Shoemake's paper
	theta = (float) acos(dot);
	theta = theta < 0.0 ? -theta : theta;

	st = (float) sin(theta);
	sut = (float) sin(factor*theta);
	sout = (float) sin((1-factor)*theta);
	interp1 = sout/st;
	interp2 = sut/st;

	Quaternion result;
	result.x = interp1*q1.x + interp2*q2.x;
	result.y = interp1*q1.y + interp2*q2.y;
	result.z = interp1*q1.z + interp2*q2.z;
	result.w = interp1*q1.w + interp2*q2.w;

	result.NormalizeIt();
	return result;
}

/** Converts a quaternion to a angle-axis rotation.
@param axis The returned axis for the rotation.
@param angle The returned angle for the rotation, in radians. */
void Quaternion::ToAngleAxis(Vector3D& axis, float& angle) const{
	angle = 2.0f * acosf(w);
	float s = sinf(angle / 2.0f);
	if (s != 0.0f)
	{
		axis.x = x / s;
		axis.y = y / s;
		axis.z = z / s;
		axis.Unitize();

	}
	else
	{
		// If s == 0, then angle == 0 and there is no rotation: assign any axis.
		axis = Vector3D(1.0f,0.0f,0.0f);
	}

}

/** Converts a quaternion to a transformation matrix.
@return The transformation matrix for this quaternion. */
Matrix Quaternion::ToMatrix() const{
	Matrix tm;
	SetToMatrix(tm);
	return tm;
}

/** Sets the Quaternions transform onto the specified matrix.
This will overwrite any existing rotations, but not positions
@param m  The matrix to set our transform onto */
void Quaternion::SetToMatrix(Matrix& m) const{
	float x,y,z,w,l;
	l = this->Length();
	if(l==0.0f)l = 1.0f;
	else l=1/l;
	x = this->x * l;
	y = this->y * l;
	z = this->z * l ;
	w = this->w * l;
	
	m[0] = 1.0f - 2*y*y - 2*z*z;
	m[4] = 2*x*y - 2*z*w; 
	m[8] = 2*x*z +2*y*w; 

	m[1] = 2*x*y + 2*z*w; 
	m[5] = 1.0f - 2*x*x - 2*z*z;
	m[9] = 2*y*z - 2*x*w; 

	m[2] = 2*x*z - 2*y*w; 
	m[6] = 2*y*z + 2*x*w; 
	m[10] = 1.0f - 2*x*x - 2*y*y;
}

/** Get the quaternion that represents the matrix rotation
@param mat The matrix whose rotation we will represent */
Quaternion Quaternion::MatrixRotationQuaternion(const Matrix& mat){
	Quaternion q;
	/*
	float tr,s;

	tr = 1.0f + mat[0] + mat[5] + mat[10];
	if (tr > 0.00001f)
	{
		s = sqrtf(tr) * 2.0f;
		q.x = (mat[6] - mat[9]) / s;
		q.y = (mat[8] - mat[2]) / s;
		q.z = (mat[1] - mat[4]) / s;
		q.w = s * 0.25f;
	}
	else if (mat[0] > mat[5])
	{
		s = sqrtf(1.0f + mat[0] - mat[5] - mat[10]) * 2.0f;
		q.x = 0.25f * s;
		q.y = (mat[1] + mat[4]) / s;
		q.z = (mat[8] + mat[2]) / s;
		q.w = (mat[6] - mat[9]) / s;
	}
	else if (mat[5] > mat[10])
	{
		s = sqrtf(1.0f + mat[5] - mat[0] - mat[10]) * 2.0f;
		q.x = (mat[1] + mat[4]) / s;
		q.y = 0.25f * s;
		q.z = (mat[6] + mat[9]) / s;
		q.w = (mat[8] - mat[2]) / s;
	}
	else
	{
		s  = sqrtf(1.0f + mat[10] - mat[0] - mat[5]) * 2.0f;
		q.x = (mat[8] + mat[2]) / s;
		q.y = (mat[6] + mat[9]) / s;
		q.z = 0.25f * s;
		q.w = (mat[1] - mat[4]) / s;
	}*/
	float trace = mat[0] + mat[5] + mat[10];
	if( trace > 0 ) {// I changed M_EPSILON to 0
		float s = 0.5f / sqrtf(trace+ 1.0f);
		q.w = 0.25f / s;
		q.x = ( mat[6] - mat[9] ) * s;
		q.y = ( mat[8] - mat[2] ) * s;
		q.z = ( mat[1] - mat[4] ) * s;
	} else {
		if ( mat[0] > mat[5] && mat[0] > mat[10] ) {
			float s = 2.0f * sqrtf( 1.0f + mat[0] - mat[5] - mat[10]);
			q.w = (mat[6] - mat[9] ) / s;
			q.x = 0.25f * s;
			q.y = (mat[4] + mat[1] ) / s;
			q.z = (mat[8] + mat[2] ) / s;
		} else if (mat[5] > mat[10]) {
			float s = 2.0f * sqrtf( 1.0f + mat[5] - mat[0] - mat[10]);
			q.w = (mat[8] - mat[2] ) / s;
			q.x = (mat[4] + mat[1] ) / s;
			q.y = 0.25f * s;
			q.z = (mat[9] + mat[6] ) / s;
		} else {
			float s = 2.0f * sqrtf( 1.0f + mat[10] - mat[0] - mat[5] );
			q.w = (mat[1] - mat[4] ) / s;
			q.x = (mat[8] + mat[2] ) / s;
			q.y = (mat[9] + mat[6] ) / s;
			q.z = 0.25f * s;
		}
	}
	return q;
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float factor){
	return q1.slerp(q2,factor);
}