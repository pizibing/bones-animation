#ifndef CHAR_MATRIX_H
#define CHAR_MATRIX_H

class Quaternion;
class Vector3D;

// column first 4*4 matrix for vertex transform

class Matrix
{
	// member variables
public:
	/* column first 4*4 matrix
    	m0  m4  m8   m12 (rotate x )
		m1  m5  m9   m13 (rotate y)
		m2  m6  m10  m14 (rotate z)
		m3  m7  m11  m15 (translation)*/
	float m[16];
	
	// constructors/destructor
public:
	inline Matrix()	{
		for(int i=0;i<16;i++)m[i]=0.0f;
		for(int i=0;i<16;i+=4)m[i]=1.0f;
	}

	inline Matrix(const Matrix & matrix)	{
		for(int i=0;i<16;i++)
			m[i] = matrix.m[i];
	}

	inline ~Matrix(){}

	// member functions	

	// [] operation to get component values
	inline float& operator[](unsigned int index)
	{
		return m[index];
	}
	// const [] operation to get component values
	inline const float& operator[](unsigned int index) const
	{
		return m[index];
	}	
	
	Matrix operator * (const Matrix & matrix) const;

	inline void operator *= (const Matrix & matrix);

	inline Matrix operator * (float factor) const;

	inline void operator *= (float factor);

	// set the matrix with the given rotation and translation
	inline void set(const Quaternion rotation, Vector3D translation);
	// set the matrix with the given rotation ,translation will not be changed
	inline void set(const Quaternion rotation);
	// set the matrix with the given translation, rotation will not be changed
	inline void set(Vector3D translation);

	// @return the rotation of the matrix
	inline Quaternion getRotation() const;

	// @return the translation of the matrix
	inline Vector3D getTranslation() const;

	// @return the inverse matrix of the matrix
	Matrix getInverseMatrix() const;

	// transform the position by the matrix
	// @param position float array for a vector3d
	inline void transform(float *position) const;

	inline Vector3D TransformVector(const Vector3D& v) const;
	

public:	
	// The identity matrix.
	static const Quaternion Identity;
	
};


#endif