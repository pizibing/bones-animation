 // Vector3d.cpp: implementation of the Vector3D class.   
//#include "stdafx.h" .  
#include "Vector3D.h"   
    
 //////////////////////////////////////////////////////////////////////   
 // Construction/Destruction   
 //////////////////////////////////////////////////////////////////////   
       
 Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f)   
 {}   
    
  
 Vector3D::Vector3D(float x, float y, float z)   
 {   
     this->x = x;   
     this->y = y;   
     this->z = z;   
 }   
    
 Vector3D::Vector3D(const VECTOR3D& vector)   
 {   
     x = vector.x;   
     y = vector.y;   
     z = vector.z;   
 }   
    
    
 Vector3D::~Vector3D()   
 {}   
    
 //////////////////////////////////////////////////////////////////////////////////////////////   
 //   
 //                    set and get
 //   
 //////////////////////////////////////////////////////////////////////////////////////////////   
    
 //set 
 void Vector3D::Set(float x, float y, float z)   
 {   
     this->x = x;   
     this->y = y;   
     this->z = z;   
 }   
    
 //get  
 VECTOR3D Vector3D::Get() const   
 {   
     VECTOR3D vector;   
     vector.x = x;   
     vector.y = y;   
     vector.z = z;   
     return vector;   
 }   
    
    
 ///////////////////////////   scale   ////////////////////////////////   
    
 //scale a vector   
 void Vector3D::Scale(Vector3D vector)   
 {   
     x *= vector.x;   
     y *= vector.y;   
     z *= vector.z;   
 }   
    
 //scale  a VECTOR  
 void Vector3D::Scale(VECTOR3D vector)   
 {   
     x *= vector.x;   
     y *= vector.y;   
     z *= vector.z;   
 }   
    
 //scale this vector with a vector 
 void Vector3D::Scale(float x,  float y,  float z)   
 {   
     this->x *=  x;   
     this->y *=  y;   
     this->z *=  z;   
 }   
    
 //rotate the vector with the x axis
 void Vector3D::RotateX(float fTheta)   
 {    
     float fRad = (float)((fTheta * PIE) / 180.0);      
     float yy = (float)(y * cos(fRad) - z * sin(fRad));   
     float zz = (float)(y * sin(fRad) + z * cos(fRad));   
     y = yy;   
     z = zz;   
 }   
    
 //rotate the vector with the y axis  
 void Vector3D::RotateY(float fTheta)   
 {     
     float fRad = (float)((fTheta * PIE) / 180.0);      
     float xx = (float)(x * cos(fRad) + z * sin(fRad));   
     float zz = (float)(-x * sin(fRad) + z * cos(fRad));   
     x = xx;   
     z = zz;   
 }   
    
 //rotate the vector with the z axis 
 void Vector3D::RotateZ(float fTheta)   
 {    
     float fRad = (float)((fTheta * PIE) / 180.0);      
     float xx = (float)(x * cos(fRad) - y * sin(fRad));   
     float yy = (float)(x * sin(fRad) + y * cos(fRad));   
     x = xx;   
     y = yy;   
 }   
    
 //rotate the vector with any axis 
 void Vector3D::Rotate(float fTheta,  Vector3D axis)   
 {   
     //unitize
     axis.Unitize();   
    
  
     float fx = axis.x,  fy = axis.y,  fz = axis.z;   
          
     float fRad = (float)((fTheta * PIE) / 180.0);   
    
     float c = (float)cos(fRad);   
     float s = (float)sin(fRad);   
    
       
       
     float xx = (fx * fx * (1.0f - c) + c)      * x +   
                (fx * fy * (1.0f - c) - fz * s) * y +   
                (fx * fz * (1.0f - c) + fy * s) * z;   
    
     float yy = (fy * fx * (1.0f - c) + fz * s) * x +   
                (fy * fy * (1.0f - c) + c)      * y +   
                (fy * fz * (1.0f - c) - fx * s) * z;   
    
     float zz = (fz * fx * (1.0f - c) - fy * s) * x +    
                (fz * fy * (1.0f - c) + fx * s) * y +   
                (fz * fz * (1.0f - c) + c)      * z;   
    
     //return  
     x = xx;   
     y = yy;   
     z = zz;   
 }   
    
 //rotate the vector with any axis 
 void Vector3D::Rotate(float fTheta,  VECTOR3D axis)   
 {   
     Vector3D vector = axis;   
     this->Rotate(fTheta, vector);   
 }   
    
 //(x, y, z) is the retate vector   
 void Vector3D::Rotate(float fTheta,  float x, float y, float z)   
 {   
     Vector3D vector(x, y, z);   
     this->Rotate(fTheta, vector);   
 }   
    
 //////////////////////////////////////////////////////////////////////////////////////////////   
 //   
 //                    get the facet normal  
 //   
 //////////////////////////////////////////////////////////////////////////////////////////////   
    
    
 //get the facet normal  
 VECTOR3D Vector3D::Noraml(const VERTEX3D& v1,  const VERTEX3D& v2,  const VERTEX3D& v3)   
 {   
     //right hand rule 
     Vector3D vector1((v2.x - v1.x),  (v2.y - v1.y),  (v2.z - v1.z));    
     Vector3D vector2((v3.x - v1.x),  (v3.y - v1.y),  (v3.z - v1.z));    
     Vector3D noraml = vector1 * vector2;   
     VECTOR3D n = {noraml.x,  noraml.y,  noraml.z};   
     return n;   
 }    
    
 //get the facet normal
 VECTOR3D Vector3D::Noraml(const HOMOCOORD& v1,  const HOMOCOORD& v2,  const HOMOCOORD& v3)   
 {   
     //if the triangle contains a vertex in the remote enough place,the triangle will be a abnomal triangle
	 float w1 = v1.w,  w2 = v2.w,  w3 = v3.w;   
     if(ABS(w1) <= 0.00001f) w1 = 1.0f;   
     if(ABS(w2) <= 0.00001f) w2 = 1.0f;   
     if(ABS(w3) <= 0.00001f) w3 = 1.0f;   
    
     float x1 = v1.x / w1,  y1 = v1.y / w1,  z1 = v1.z / w1;   
     float x2 = v2.x / w2,  y2 = v2.y / w2,  z2 = v2.z / w2;   
     float x3 = v3.x / w3,  y3 = v3.y / w3,  z3 = v3.z / w3;   
    
     Vector3D vector1((x2 - x1),  (y2 - y1),  (z2 - z1));    
     Vector3D vector2((x3 - x1),  (y3 - y1),  (z3 - z1));    
     Vector3D noraml = vector1 * vector2;   
     VECTOR3D n = {noraml.x,  noraml.y,  noraml.z};   
     return n;   
 }