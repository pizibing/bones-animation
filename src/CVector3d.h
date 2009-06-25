///////////////////////////////////////////////////////////////////////////////// 
//	 
// Vector3d.h: interface for the CVector3d class. 
// 
//////////////////////////////////////////////////////////////////////////////// 
// ��Ȩ����(2002) 
// Copyright(2002) 
// ��д��: ������ 
// Author: Xiang Shiming 
 
 
//��ά�������� 
 
#ifndef _CVECTOR3D_H 
#define _CVECTOR3D_H 
#include "Grphcs.h" 
#include "math.h" 
 
class CVector3d   
{ 
public: 
	CVector3d(); 
	CVector3d(float x,float y,float z); 
	CVector3d(const VECTOR3D& vector); 
	virtual ~CVector3d(); 
 
public: 
	float x,y,z; 
 
public: 
	//�������ȡ���� 
	void Set(float x,float y,float z); 
	VECTOR3D Get() const; 
 
public: 
	//���������(=) 
	inline CVector3d operator = (const CVector3d& vector); 
	inline CVector3d operator = (const VECTOR3D& vector); 
	 
	//���������(+),������һ��CVector3d 
	inline CVector3d operator + (const CVector3d& vector); 
	inline CVector3d operator + (const VECTOR3D& vector);		//�����ɲ�����(�ṹû������ "+") 
 
	//���������(+=) 
	inline CVector3d operator += (const CVector3d& vector); 
	inline CVector3d operator += (const VECTOR3D& vector);		 
 
	//���������(-),����Ϊ����,������һ��CVector3d 
	inline CVector3d operator - (const CVector3d& vector); 
	inline CVector3d operator - (const VECTOR3D& vector);		 
	 
	//���������(-),����Ϊ���� 
	inline CVector3d operator -= (const CVector3d& vector); 
	inline CVector3d operator -= (const VECTOR3D& vector);		 
 
	//���������(*),����Ϊ����,������һ��CVector3d 
	//������� 
	inline CVector3d operator * (const CVector3d& vector); 
	inline CVector3d operator * (const VECTOR3D& vector);		 
	//������������ 
	inline CVector3d operator * (float fs); 
 
	//���������(*),����Ϊ����,������һ��CVector3d 
	//������� 
	inline CVector3d operator *= (const CVector3d& vector); 
	inline CVector3d operator *= (const VECTOR3D& vector);		 
	//������������ 
	inline CVector3d operator *= (float fs); 
 
	//����"/"�����,û�б�Ҫ���� 
 
public: 
	//������� 
	inline float Dot(const CVector3d& vector); 
	inline float Dot(const VECTOR3D& vector); 
	inline float Dot(float x,float y, float z); 
 
	//������ģ 
	inline float Mag(); 
	inline float MagSquared(); 
 
	//����ʸ���˵�֮��ľ��� 
	inline float Dist(const CVector3d& vector); 
	inline float Dist(const VECTOR3D& vector); 
	inline float Dist(float x,float y,float z); 
 
	//������λ�� 
	inline CVector3d Unit(); 
	inline void Unitize(); 
	 
public: 
 
	//��this�������� 
	void Scale(CVector3d vector); 
	void Scale(VECTOR3D vector); 
	void Scale(float x, float y, float z); 
 
	//��this������������ת,fThetaΪ�Ƕȵ�λ 
	void RotateX(float fTheta); 
	void RotateY(float fTheta); 
	void RotateZ(float fTheta); 
 
	//��this��������������ת,fThetaΪ�Ƕȵ�λ 
	void Rotate(float fTheta, CVector3d axis); 
	void Rotate(float fTheta, VECTOR3D axis); 
	//(x,y,z)Ϊ��ת������ 
	void Rotate(float fTheta, float x, float y, float z); 
 
	//����С�淨�� 
	VECTOR3D Noraml(const VERTEX3D& v1, const VERTEX3D& v2, const VERTEX3D& v3); 
	VECTOR3D Noraml(const HOMOCOORD& v1, const HOMOCOORD& v2, const HOMOCOORD& v3); 
 
}; 
#endif  
 
////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//					  ���������   
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
 
//���������(=) 
//����ΪCVector3d���� 
inline CVector3d CVector3d::operator = (const CVector3d& vector) 
{ 
	x = vector.x; 
	y = vector.y; 
	z = vector.z; 
	return *this; 
} 
 
//���������(=) 
//����ΪVECTOR3D�ṹ 
inline CVector3d CVector3d::operator = (const VECTOR3D& vector) 
{ 
	x = vector.x; 
	y = vector.y; 
	z = vector.z; 
	return *this; 
} 
 
//���������(+) 
//����ΪCVector3d����,������һ��CVector3d���� 
inline CVector3d CVector3d::operator + (const CVector3d& vector) 
{ 
	CVector3d v; 
	v.x = x + vector.x; 
	v.y = y + vector.y; 
	v.z = z + vector.z; 
	return v; 
} 
 
 
//���������(+) 
//����ΪVECTOR3D�ṹ,������һ��CVector3d���� 
inline CVector3d CVector3d::operator + (const VECTOR3D& vector) 
{ 
	CVector3d v; 
	v.x = x + vector.x; 
	v.y = y + vector.y; 
	v.z = z + vector.z; 
	return v; 
} 
 
 
//���������(+=) 
//����ΪCVector3d���� 
inline CVector3d CVector3d::operator += (const CVector3d& vector) 
{ 
	x += vector.x; 
	y += vector.y; 
	z += vector.z; 
	return *this; 
} 
 
//���������(+=) 
//����ΪVECTOR3D�ṹ 
inline CVector3d CVector3d::operator += (const VECTOR3D& vector) 
{ 
	x += vector.x; 
	y += vector.y; 
	z += vector.z; 
	return *this; 
} 
 
//���������(-) 
//����ΪCVector3d����,Ϊ����,������һ��CVector3d���� 
inline CVector3d CVector3d::operator - (const CVector3d& vector) 
{ 
	CVector3d v; 
	v.x = x - vector.x; 
	v.y = y - vector.y; 
	v.z = z - vector.z; 
	return v; 
} 
 
//���������(-) 
//����ΪVECTOR3D�ṹ,Ϊ����,������һ��CVector3d���� 
inline CVector3d CVector3d::operator - (const VECTOR3D& vector) 
{ 
	CVector3d v; 
	v.x = x - vector.x; 
	v.y = y - vector.y; 
	v.z = z - vector.z; 
	return v; 
} 
 
//���������(-=) 
//����ΪCVector3d����,Ϊ���� 
inline CVector3d CVector3d::operator -= (const CVector3d& vector) 
{ 
	x -= vector.x; 
	y -= vector.y; 
	z -= vector.z; 
	return *this; 
} 
 
//���������(-=) 
//����ΪVECTOR3D�ṹ,Ϊ���� 
inline CVector3d CVector3d::operator -= (const VECTOR3D& vector) 
{ 
	x -= vector.x; 
	y -= vector.y; 
	z -= vector.z; 
	return *this; 
} 
 
//���������(*),��������� 
//����ΪCVector3d����,Ϊ����,������һ��CVector3d���� 
inline CVector3d CVector3d::operator * (const CVector3d& vector) 
{ 
	CVector3d v; 
	v.x = (y * vector.z - z * vector.y); 
	v.y = (z * vector.x - x * vector.z); 
	v.z = (x * vector.y - y * vector.x); 
	return v; 
} 
 
//���������(*),��������� 
//����ΪVECTOR3D�ṹ,Ϊ����,������һ��CVector3d���� 
inline CVector3d CVector3d::operator * (const VECTOR3D& vector) 
{ 
	CVector3d v; 
	v.x = (y * vector.z - z * vector.y); 
	v.y = (z * vector.x - x * vector.z); 
	v.z = (x * vector.y - y * vector.x); 
	return v; 
} 
 
//���������(*),����������֮�� 
//����Ϊ�������,������һ��CVector3d���� 
inline CVector3d CVector3d::operator * (float fs) 
{ 
	CVector3d v; 
	v.x = x * fs; 
	v.y = y * fs; 
	v.z = z * fs; 
	return v; 
} 
 
//���������(*=),��������� 
//����ΪCVector3d����,Ϊ����, 
inline CVector3d CVector3d::operator *= (const CVector3d& vector) 
{ 
	float xx = (y * vector.z - z * vector.y); 
	float yy = (z * vector.x - x * vector.z); 
	float zz = (x * vector.y - y * vector.x); 
	x = xx; 
	y = yy; 
	z = zz; 
	return *this; 
} 
 
//���������(*=),��������� 
//����ΪVECTOR3D�ṹ,Ϊ���� 
inline CVector3d CVector3d::operator *= (const VECTOR3D& vector) 
{ 
	float xx = (y * vector.z - z * vector.y); 
	float yy = (z * vector.x - x * vector.z); 
	float zz = (x * vector.y - y * vector.x); 
	x = xx; 
	y = yy; 
	z = zz; 
	return *this; 
} 
 
//���������(*),����������֮�� 
//����Ϊ������� 
inline CVector3d CVector3d::operator *= (float fs) 
{ 
	x *= fs; 
	y *= fs; 
	z *= fs; 
	return *this; 
} 
 
////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//					  ������������   
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
 
//�����ڻ� 
inline float CVector3d::Dot(const CVector3d& vector) 
{ 
	return (x * vector.x + y * vector.y + z * vector.z); 
} 
 
//�����ڻ� 
inline float CVector3d::Dot(const VECTOR3D& vector) 
{ 
	return (x * vector.x + y * vector.y + z * vector.z); 
} 
 
//�����ڻ� 
inline float CVector3d::Dot(float x,float y, float z) 
{ 
	return (this->x * x + this->y * y + this->z * z); 
} 
 
//������ģ 
inline float CVector3d::Mag() 
{ 
	return (float)(sqrt(x * x + y * y + z * z)); 
} 
 
//����ģ��ƽ�� 
inline float CVector3d::MagSquared() 
{ 
	return (x * x + y * y + z * z); 
} 
 
//������ʸ���˵�֮��ľ��� 
inline float CVector3d::Dist(const CVector3d& vector) 
{ 
	return (*this - vector).Mag(); 
} 
 
//������ʸ���˵�֮��ľ��� 
inline float CVector3d::Dist(const VECTOR3D& vector) 
{ 
	return (*this - vector).Mag(); 
} 
 
//������ʸ���˵�֮��ľ��� 
inline float CVector3d::Dist(float x,float y,float z) 
{ 
	return (float)sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y) + (this->z - z) * (this->z - z));  
} 
 
//������λ�� 
//������λ��,����һ����λ���� 
inline CVector3d CVector3d::Unit() 
{ 
	CVector3d vector; 
	float fMag = (float)sqrt(x * x + y * y + z * z); 
	if(fMag < 0.0001f)fMag = 1.0f; 
	vector.x = x / fMag; 
	vector.y = y / fMag; 
	vector.z = z / fMag; 
	return vector; 
} 
 
//����λ�� 
inline void CVector3d::Unitize() 
{ 
	float fMag = (float)sqrt(x * x + y * y + z * z); 
	if(fMag < 0.0001f)fMag = 1.0f; 
	x /= fMag; 
	y /= fMag; 
	z /= fMag; 
}