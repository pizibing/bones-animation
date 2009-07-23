#ifndef CHAR_QUATERNION_H
#define CHAR_QUATERNION_H

#include <math.h>

class Matrix;
class Vector3D;

class Quaternion{

	// member variables
public:
	// the i component
	float x;
	// the j component
	float y;
	// the k component
	float z;
	// the scalar component
	float w;

	// constructors/destructor
public:
	// creates a empty quaternion
	inline Quaternion():x(0.0f),y(0.0f),z(0.0f),w(1.0f){}
	// creates the quaternion with the given component values
	inline Quaternion(float qx, float qy, float qz, float qw): x(qx),y(qy),z(qz),w(qw){}
	// creates the quaternion with the given quaternion
	inline Quaternion(const Quaternion& q): x(q.x), y(q.y), z(q.z), w(q.w) {};

	inline ~Quaternion() {};

	// member functions
	
	// [] operation to get component values
	// x = [0], y = [1], z = [2], w = [3]
	inline float& operator[](unsigned int index)
	{
		return (&x)[index];
	}
	// const [] operation to get component values
	// x = [0], y = [1], z = [2], w = [3]
	inline const float& operator[](unsigned int index) const
	{
		return (&x)[index];
	}	

	// copy operation with the given quaternion
	inline void operator=(const Quaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	/**	Retrieves the squared length of the vector.
	@return The squared length of this vector. */
	inline float LengthSquared() const { return x * x + y * y + z * z + w * w; }

	/**	Retrieves the length of the vector.
	@return The length of this vector. */
	inline float Length() const { return sqrtf(x * x + y * y + z * z + w * w); }

	/** Normalizes this vector. */
	inline void NormalizeIt() { float l = Length(); if (l > 0.0f) { x /= l; y /= l; z /= l; w /= l; }}

	/** Returns a quaternion with a unit length at the same space as this quaternion
	@return a quaternion with length 1 and same direction as this quaternion. */
	inline Quaternion Normalize() const { float l = Length(); return Quaternion(x / l, y / l, z / l, w / l); }

	// rotate the quaternion by another q.
	// @return a quaternion that rotate by q then by this
	Quaternion operator*(const Quaternion& q) const;

	// rotate the vector3d by this quaternion
	Vector3D operator*(const Vector3D& v) const;

	/** Returns the conjugate of this quaternion.
	@return The conjugate. */
	inline Quaternion operator ~() const { return Quaternion(-x, -y, -z, w); }

	/** Applies quaternion multiplication of the given quaternion with this
	quaternion and returns the value.
	@param q The quaternion multiplied with. */
	inline void operator*=(const Quaternion& q) { (*this) = (*this) * q; }

	/** Returns the slerp of this quaternion to other at factor time
	@param other The Quaternion to interpolate to
	@param factor The percentage (0 < time < 1) to slerp
	@return The Quaternion formed by the slerp */
	Quaternion slerp(const Quaternion& other, float factor) const;

	/** Converts a quaternion to a angle-axis rotation.
	@param axis The returned axis for the rotation.
	@param angle The returned angle for the rotation, in radians. */
	void ToAngleAxis(Vector3D& axis, float& angle) const;

	/** Converts a quaternion to a transformation matrix.
	@return The transformation matrix for this quaternion. */
	Matrix ToMatrix() const;

	/** Sets the Quaternions transform onto the specified matrix.
	This will overwrite any existing rotations, but not positions
	@param m  The matrix to set our transform onto */
	void SetToMatrix(Matrix& m) const;

	/** Get the quaternion that represents the matrix rotation
	@param mat The matrix whose rotation we will represent */
	static Quaternion MatrixRotationQuaternion(const Matrix& matrix);

	/** Returns the slerp of this quaternion to other at factor time
	@param q1 The first Quaternion 
	@param q2 The Quaternion to interpolate to
	@param factor The percentage (0 < time < 1) to slerp
	@return The Quaternion formed by the slerp */
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float factor);

};

Vector3D operator*(const Vector3D& v, const Quaternion& q);


#endif