///////////////////////////////////////////////////////////////////////////////// 
//	 
// Vector3d.h: interface for the Vector3D class. 
// 
//////////////////////////////////////////////////////////////////////////////// 
 
 
//Vector3D
 
#ifndef _Vector3D_H 
#define _Vector3D_H 
#include "Grphcs.h" 
#include "math.h" 
 
class Vector3D   
{ 
public: 
	Vector3D(); 
	Vector3D(float x,float y,float z); 
	Vector3D(const VECTOR3D& vector); 
	virtual ~Vector3D(); 
 
public: 
	float x,y,z; 
 
public: 
	//set and get 
	void Set(float x,float y,float z); 
	VECTOR3D Get() const; 
 
public: 
	//reload (=) 
	inline Vector3D operator = (const Vector3D& vector); 
	inline Vector3D operator = (const VECTOR3D& vector); 
	 
	//reload (+)
	inline Vector3D operator + (const Vector3D& vector); 
	inline Vector3D operator + (const VECTOR3D& vector);		//交换律不成立(结构没有重载 "+") 
 
	//reload (+=) 
	inline Vector3D operator += (const Vector3D& vector); 
	inline Vector3D operator += (const VECTOR3D& vector);		 
 
	//reload(-) 
	inline Vector3D operator - (const Vector3D& vector); 
	inline Vector3D operator - (const VECTOR3D& vector);		 
	 
	//reload(-)
	inline Vector3D operator -= (const Vector3D& vector); 
	inline Vector3D operator -= (const VECTOR3D& vector);		 
 
	//reload(*)
	inline Vector3D operator * (const Vector3D& vector); 
	inline Vector3D operator * (const VECTOR3D& vector);		 
	//scalar*vector
	inline Vector3D operator * (float fs); 
 
	//reload(*)
	inline Vector3D operator *= (const Vector3D& vector); 
	inline Vector3D operator *= (const VECTOR3D& vector);		 
	//scalar*vector
	inline Vector3D operator *= (float fs); 
 
 
public: 
	//vector dot float
	inline float Dot(const Vector3D& vector); 
	inline float Dot(const VECTOR3D& vector); 
	inline float Dot(float x,float y, float z); 
 
	//module of vector
	inline float Mag(); 
	inline float MagSquared(); 
 
	//the distance from the begining to the end of the vector
	inline float Dist(const Vector3D& vector); 
	inline float Dist(const VECTOR3D& vector); 
	inline float Dist(float x,float y,float z); 
 
	//unit the vector
	inline Vector3D Unit(); 
	inline void Unitize(); 
	 
public: 
 
	//scale the vector
	void Scale(Vector3D vector); 
	void Scale(VECTOR3D vector); 
	void Scale(float x, float y, float z); 
 
	//rotate with x axis
	void RotateX(float fTheta); 
	void RotateY(float fTheta); 
	void RotateZ(float fTheta); 
 
	//rotate with any axis 
	void Rotate(float fTheta, Vector3D axis); 
	void Rotate(float fTheta, VECTOR3D axis); 
	//rotate with (x,y,z)
	void Rotate(float fTheta, float x, float y, float z); 
 
	//get the normal 
	VECTOR3D Noraml(const VERTEX3D& v1, const VERTEX3D& v2, const VERTEX3D& v3); 
	VECTOR3D Noraml(const HOMOCOORD& v1, const HOMOCOORD& v2, const HOMOCOORD& v3); 
 
}; 
#endif  
 
////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//					  reload  
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
 
//reload (=)
inline Vector3D Vector3D::operator = (const Vector3D& vector) 
{ 
	x = vector.x; 
	y = vector.y; 
	z = vector.z; 
	return *this; 
} 
 
//reload(=)  
inline Vector3D Vector3D::operator = (const VECTOR3D& vector) 
{ 
	x = vector.x; 
	y = vector.y; 
	z = vector.z; 
	return *this; 
} 
 
//reload(+) 
inline Vector3D Vector3D::operator + (const Vector3D& vector) 
{ 
	Vector3D v; 
	v.x = x + vector.x; 
	v.y = y + vector.y; 
	v.z = z + vector.z; 
	return v; 
} 

//reload(+) 
inline Vector3D operator + (const Vector3D& vector1, const Vector3D& vector2) 
{ 
	Vector3D v; 
	v.x = vector1.x + vector2.x; 
	v.y = vector1.y + vector2.y; 
	v.z = vector1.z + vector2.z; 
	return v; 
} 

 
//reload(+)  
inline Vector3D Vector3D::operator + (const VECTOR3D& vector) 
{ 
	Vector3D v; 
	v.x = x + vector.x; 
	v.y = y + vector.y; 
	v.z = z + vector.z; 
	return v; 
} 
 
 
//reload(+=) 
inline Vector3D Vector3D::operator += (const Vector3D& vector) 
{ 
	x += vector.x; 
	y += vector.y; 
	z += vector.z; 
	return *this; 
} 
 
//reload(+=) 
inline Vector3D Vector3D::operator += (const VECTOR3D& vector) 
{ 
	x += vector.x; 
	y += vector.y; 
	z += vector.z; 
	return *this; 
} 
 
//reload(-) 
inline Vector3D Vector3D::operator - (const Vector3D& vector) 
{ 
	Vector3D v; 
	v.x = x - vector.x; 
	v.y = y - vector.y; 
	v.z = z - vector.z; 
	return v; 
} 
 
//reload(-) 
inline Vector3D Vector3D::operator - (const VECTOR3D& vector) 
{ 
	Vector3D v; 
	v.x = x - vector.x; 
	v.y = y - vector.y; 
	v.z = z - vector.z; 
	return v; 
} 
 
//reload(-=) 
inline Vector3D Vector3D::operator -= (const Vector3D& vector) 
{ 
	x -= vector.x; 
	y -= vector.y; 
	z -= vector.z; 
	return *this; 
} 
 
//reload(-=) 
inline Vector3D Vector3D::operator -= (const VECTOR3D& vector) 
{ 
	x -= vector.x; 
	y -= vector.y; 
	z -= vector.z; 
	return *this; 
} 
 
//reload(*)
inline Vector3D Vector3D::operator * (const Vector3D& vector) 
{ 
	Vector3D v; 
	v.x = (y * vector.z - z * vector.y); 
	v.y = (z * vector.x - x * vector.z); 
	v.z = (x * vector.y - y * vector.x); 
	return v; 
} 
 
//reload(*)
inline Vector3D Vector3D::operator * (const VECTOR3D& vector) 
{ 
	Vector3D v; 
	v.x = (y * vector.z - z * vector.y); 
	v.y = (z * vector.x - x * vector.z); 
	v.z = (x * vector.y - y * vector.x); 
	return v; 
} 
 
//reload(*)
inline Vector3D Vector3D::operator * (float fs) 
{ 
	Vector3D v; 
	v.x = x * fs; 
	v.y = y * fs; 
	v.z = z * fs; 
	return v; 
} 

//reload(*)
inline Vector3D operator * (const Vector3D & vector,float fs) 
{ 
	Vector3D v; 
	v.x = vector.x * fs; 
	v.y = vector.y * fs; 
	v.z = vector.z * fs; 
	return v; 
} 
 
//reload(*=)
inline Vector3D Vector3D::operator *= (const Vector3D& vector) 
{ 
	float xx = (y * vector.z - z * vector.y); 
	float yy = (z * vector.x - x * vector.z); 
	float zz = (x * vector.y - y * vector.x); 
	x = xx; 
	y = yy; 
	z = zz; 
	return *this; 
} 
 
//reload(*=)
inline Vector3D Vector3D::operator *= (const VECTOR3D& vector) 
{ 
	float xx = (y * vector.z - z * vector.y); 
	float yy = (z * vector.x - x * vector.z); 
	float zz = (x * vector.y - y * vector.x); 
	x = xx; 
	y = yy; 
	z = zz; 
	return *this; 
} 
 
//reload(*)
inline Vector3D Vector3D::operator *= (float fs) 
{ 
	x *= fs; 
	y *= fs; 
	z *= fs; 
	return *this; 
} 
 
////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//					  some basic operation  
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
 
//vector dot vecotr
inline float Vector3D::Dot(const Vector3D& vector) 
{ 
	return (x * vector.x + y * vector.y + z * vector.z); 
} 
 
//vector dot vector
inline float Vector3D::Dot(const VECTOR3D& vector) 
{ 
	return (x * vector.x + y * vector.y + z * vector.z); 
} 
 
//vector dot vector
inline float Vector3D::Dot(float x,float y, float z) 
{ 
	return (this->x * x + this->y * y + this->z * z); 
} 
 
//the module of the vector 
inline float Vector3D::Mag() 
{ 
	return (float)(sqrt(x * x + y * y + z * z)); 
} 
 
//the squared module 
inline float Vector3D::MagSquared() 
{ 
	return (x * x + y * y + z * z); 
} 
 
//the distance from the begining to the end of the vector
inline float Vector3D::Dist(const Vector3D& vector) 
{ 
	return (*this - vector).Mag(); 
} 
 
//the distance of two vectors
inline float Vector3D::Dist(const VECTOR3D& vector) 
{ 
	return (*this - vector).Mag(); 
} 
 
//the distance of two vectors 
inline float Vector3D::Dist(float x,float y,float z) 
{ 
	return (float)sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y) + (this->z - z) * (this->z - z));  
} 
 
//unit the vector
inline Vector3D Vector3D::Unit() 
{ 
	Vector3D vector; 
	float fMag = (float)sqrt(x * x + y * y + z * z); 
	if(fMag < 0.0001f)fMag = 1.0f; 
	vector.x = x / fMag; 
	vector.y = y / fMag; 
	vector.z = z / fMag; 
	return vector; 
} 
 
//unitize 
inline void Vector3D::Unitize() 
{ 
	float fMag = (float)sqrt(x * x + y * y + z * z); 
	if(fMag < 0.0001f)fMag = 1.0f; 
	x /= fMag; 
	y /= fMag; 
	z /= fMag; 
}