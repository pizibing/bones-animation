#include "quaternion.h"
#include "matrix.h"
#include "Vector3D.h"

// rotate the quaternion by another q.
// @return a quaternion that rotate by q then by this
Quaternion Quaternion::operator*(const Quaternion& q) const{
	Quaternion r;
	r.x = q.w * x + q.x * w + q.y * z - q.z * y;
	r.y = q.w * y + q.y * w + q.z * x - q.x * z;
	r.z = q.w * z + q.z * w + q.x * y - q.y * x;
	r.w = q.w * w - q.x * x - q.y * y - q.z * z;
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
	float theta, st, sut, sout, interp1, interp2;
	float dot = x * other.x + y * other.y + z * other.z +
		w * other.w;

	if (dot == 1.0f) return *this;

	// algorithm taken from Shoemake's paper
	theta = (float) acos(dot);
	theta = theta < 0.0 ? -theta : theta;

	st = (float) sin(theta);
	sut = (float) sin(factor*theta);
	sout = (float) sin((1-factor)*theta);
	interp1 = sout/st;
	interp2 = sut/st;

	Quaternion result;
	result.x = interp1*x + interp2*other.x;
	result.y = interp1*y + interp2*other.y;
	result.z = interp1*z + interp2*other.z;
	result.w = interp1*w + interp2*other.w;

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
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz, den;

	// For unit q, just set s = 2.0; or or set xs = q.x + q.x, etc 
	den =  (x*x + y*y + z*z + w*w);
	if (den==0.0) {  s = (float)1.0; }
	else s = (float)2.0/den;

	xs = x * s;   ys = y * s;  zs = z * s;
	wx = w * xs;  wy = w * ys; wz = w * zs;
	xx = x * xs;  xy = x * ys; xz = x * zs;
	yy = y * ys;  yz = y * zs; zz = z * zs;

	m[0] = (float)1.0 - (yy +zz);
	m[1] = xy - wz; 
	m[2] = xz + wy; 

	m[4] = xy + wz; 
	m[5] = (float)1.0 - (xx +zz);
	m[6] = yz - wx; 

	m[8] = xz - wy; 
	m[9] = yz + wx; 
	m[10] = (float)1.0 - (xx + yy);
}

/** Get the quaternion that represents the matrix rotation
@param mat The matrix whose rotation we will represent */
Quaternion Quaternion::MatrixRotationQuaternion(const Matrix& m){
	Quaternion q;

	float tr,s;

	tr = 1.0f + m[0] + m[5] + m[10];
	if (tr > 0.00001f)
	{
		s = sqrtf(tr) * 2.0f;
		q.x = (m[9] - m[6]) / s;
		q.y = (m[2] - m[8]) / s;
		q.z = (m[4] - m[1]) / s;
		q.w = s * 0.25f;
	}
	else if (m[0] > m[5])
	{
		s = sqrtf(1.0f + m[0] - m[5] - m[10]) * 2.0f;
		q.x = 0.25f * s;
		q.y = (m[4] + m[1]) / s;
		q.z = (m[2] + m[8]) / s;
		q.w = (m[9] - m[6]) / s;
	}
	else if (m[5] > m[10])
	{
		s = sqrtf(1.0f + m[5] - m[0] - m[10]) * 2.0f;
		q.x = (m[4] + m[1]) / s;
		q.y = 0.25f * s;
		q.z = (m[9] + m[6]) / s;
		q.w = (m[2] - m[8]) / s;
	}
	else
	{
		s  = sqrtf(1.0f + m[10] - m[0] - m[5]) * 2.0f;
		q.x = (m[2] + m[8]) / s;
		q.y = (m[9] + m[6]) / s;
		q.z = 0.25f * s;
		q.w = (m[4] - m[1]) / s;
	}
	return q;
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float factor){
	return q1.slerp(q2,factor);
}