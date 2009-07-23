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
    	m0  m4  m8   m12 
		m1  m5  m9   m13 
		m2  m6  m10  m14 
		m3  m7  m11  m15 
		Rx  Ry  Rz translation*/
	float m[16];
	
	// constructors/destructor
public:
	inline Matrix()	{
		for(int i=0;i<16;i++)m[i]=0.0f;
		for(int i=0;i<16;i+=5)m[i]=1.0f;
	}

	inline Matrix(const Matrix & matrix)	{
		for(int i=0;i<16;i++)
			m[i] = matrix.m[i];
	}

	inline Matrix(float *mv)	{
		for(int i=0;i<16;i++)m[i] = mv[i];
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

	Matrix operator + (const Matrix & matrix) const;

	void operator *= (const Matrix & matrix);

	Vector3D operator * (const Vector3D &v) const;

	Matrix operator * (float factor) const;

	void operator *= (float factor);

	// set the matrix with the given rotation and translation
	void set(const Quaternion rotation, Vector3D translation);
	// set the matrix with the given rotation ,translation will not be changed
	void set(const Quaternion rotation);
	// set the matrix with the given translation, rotation will not be changed
	void set(Vector3D translation);

	// set the matrix with a float array
	inline void set(float *mv){
		for(int i=0;i<16;i++) m[i]=mv[i];
	}

	// the float array of the matrix
	inline float * get(){
		return m;
	}

	// @return the rotation of the matrix
	Quaternion getRotation() const;

	// @return the translation of the matrix
	Vector3D getTranslation() const;

	// @return the inverse matrix of the matrix
	Matrix getInverseMatrix() const;

	// transform the position by the matrix
	// @param position float array for a vector3d
	void transform(float *position) const;

	// same as transform
	Vector3D TransformVector(const Vector3D& v) const;

	// rotate the matrix with axis (x,y,z) by angle degree
	void Rotate(float x, float y, float z, float angle);

	// rotate the matrix by quaternion
	void Rotate(const Quaternion & q);

	// rotate the matrix with by angle vector (x,y,z)
	void Translate(float x, float y, float z);

	// rotate the matrix with by angle vector3d
	void Translate(const Vector3D & v);

	// get rotation Matrix with axis(x,y,z) by angle in radians
	Matrix AxisRotationMatrix(float x, float y, float z, float angle);

public:	
	// The identity matrix.
	static const Matrix Identity;
	
};

Vector3D operator * (const Vector3D& v, const Matrix& m);

#endif