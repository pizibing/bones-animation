#ifndef CHAR_MATRIX_H
#define CHAR_MATRIX_H

	// column first 4*4 matrix for vertex transform

class Matrix
{
	// member variables
public:
	/* column first 4*4 matrix
    	m0  m4  m8   m12
		m1  m5  m9   m13
		m2  m6  m10  m14
		m3  m7  m11  m15*/
	float m[16];
	
	// constructors/destructor
public:
	inline Matrix()	{
		for(int i=0;i<16;i++)m[i]=0.0f;
		for(int i=0;i<16;i+=4)m[i]=1.0f;
	}

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
	

	
};


#endif