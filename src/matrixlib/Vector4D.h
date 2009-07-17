#ifndef VECTOR4D_H
#define VECTOR4D_H

//simple vector4D class which contain four private value and its get and set method
class Vector4D
{
public:
	float x;	/**< The first coordinate. */
	float y;	/**< The second coordinate. */
	float z;	/**< The third coordinate. */
	float w;	/**< The fourth coordinate. */

public:
	// create a Vector4D with component values
	inline Vector4D(float x, float y, float z, float w);
	// default constructor
	inline Vector4D();
	inline ~Vector4D(void){}
	
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
	inline void operator=(const Vector4D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	// set (x,y,z,w)
	inline void Set(float vx, float vy, float vz, float vw);

};

#endif