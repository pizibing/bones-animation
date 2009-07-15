#include "matrix.h"
#include "quaternion.h"

inline void Matrix::operator = (const Quaternion& q){
	float xx2=q.x*q.x*2;
	float yy2=q.y*q.y*2;
	float zz2=q.z*q.z*2;
	float xy2=q.x*q.y*2;
	float zw2=q.z*q.w*2;
	float xz2=q.x*q.z*2;
	float yw2=q.y*q.w*2;
	float yz2=q.y*q.z*2;
	float xw2=q.x*q.w*2;
	m[0]=1-yy2-zz2;   m[1]=  xy2+zw2;  m[2]=  xz2-yw2;
	m[4]=  xy2-zw2;   m[5]=1-xx2-zz2;  m[6]=  yz2+xw2;
	m[8]=  xz2+yw2;   m[9]=  yz2-xw2;  m[10]=1-xx2-yy2;
}