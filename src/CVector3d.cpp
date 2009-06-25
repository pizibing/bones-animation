# // Vector3d.cpp: implementation of the CVector3d class.   
# #include "stdafx.h"   
# #include "Vector3d.h"   
#    
# //////////////////////////////////////////////////////////////////////   
# // Construction/Destruction   
# //////////////////////////////////////////////////////////////////////   
#    
# //ȱʡ���캯��   
# CVector3d::CVector3d() : x(0.0f), y(0.0f), z(1.0f)   
# {}   
#    
# //���ع��캯��   
# CVector3d::CVector3d(float x, float y, float z)   
# {   
#     this->x = x;   
#     this->y = y;   
#     this->z = z;   
# }   
#    
# CVector3d::CVector3d(const VECTOR3D& vector)   
# {   
#     x = vector.x;   
#     y = vector.y;   
#     z = vector.z;   
# }   
#    
#    
# CVector3d::~CVector3d()   
# {}   
#    
# //////////////////////////////////////////////////////////////////////////////////////////////   
# //   
# //                    �������ȡ     
# //   
# //////////////////////////////////////////////////////////////////////////////////////////////   
#    
# //����, ���ھ���"="���ع���, �������ú���ֻ��Ҫ���²�����ʽ   
# void CVector3d::Set(float x, float y, float z)   
# {   
#     this->x = x;   
#     this->y = y;   
#     this->z = z;   
# }   
#    
# //��ȡ   
# VECTOR3D CVector3d::Get() const   
# {   
#     VECTOR3D vector;   
#     vector.x = x;   
#     vector.y = y;   
#     vector.z = z;   
#     return vector;   
# }   
#    
#    
# ///////////////////////////   �任   ////////////////////////////////   
#    
# //��������   
# //��vector������this����   
# void CVector3d::Scale(CVector3d vector)   
# {   
#     x *= vector.x;   
#     y *= vector.y;   
#     z *= vector.z;   
# }   
#    
# //��������   
# //��vector������this����   
# void CVector3d::Scale(VECTOR3D vector)   
# {   
#     x *= vector.x;   
#     y *= vector.y;   
#     z *= vector.z;   
# }   
#    
# //��������   
# //��x, y, z������this����   
# void CVector3d::Scale(float x,  float y,  float z)   
# {   
#     this->x *=  x;   
#     this->y *=  y;   
#     this->z *=  z;   
# }   
#    
# //��this����������ת   
# //����x��ת, fThetaΪ�Ƕȵ�λ   
# void CVector3d::RotateX(float fTheta)   
# {   
#     //���Ƕ�ת��Ϊ����   
#     float fRad = (float)((fTheta * PIE) / 180.0);      
#     float yy = (float)(y * cos(fRad) - z * sin(fRad));   
#     float zz = (float)(y * sin(fRad) + z * cos(fRad));   
#     y = yy;   
#     z = zz;   
# }   
#    
# //��this����������ת   
# //����y��ת, fThetaΪ�Ƕȵ�λ   
# void CVector3d::RotateY(float fTheta)   
# {   
#     //���Ƕ�ת��Ϊ����   
#     float fRad = (float)((fTheta * PIE) / 180.0);      
#     float xx = (float)(x * cos(fRad) + z * sin(fRad));   
#     float zz = (float)(-x * sin(fRad) + z * cos(fRad));   
#     x = xx;   
#     z = zz;   
# }   
#    
# //��this����������ת   
# //����y��ת, fThetaΪ�Ƕȵ�λ   
# void CVector3d::RotateZ(float fTheta)   
# {   
#     //���Ƕ�ת��Ϊ����   
#     float fRad = (float)((fTheta * PIE) / 180.0);      
#     float xx = (float)(x * cos(fRad) - y * sin(fRad));   
#     float yy = (float)(x * sin(fRad) + y * cos(fRad));   
#     x = xx;   
#     y = yy;   
# }   
#    
# //����������ת, fThetaΪ�Ƕ�,    
# //����ָ��, �������Ϊһ����������, �������������Դ�����ԭ�㻭��   
# void CVector3d::Rotate(float fTheta,  CVector3d axis)   
# {   
#     //��λ��   
#     axis.Unitize();   
#    
#     //��ת��ķ�����   
#     float fx = axis.x,  fy = axis.y,  fz = axis.z;   
#        
#     //���Ƕ�ת��Ϊ����   
#     float fRad = (float)((fTheta * PIE) / 180.0);   
#    
#     //�Ƕȵ����Һ�����   
#     float c = (float)cos(fRad);   
#     float s = (float)sin(fRad);   
#    
#    
#     //��α任�ĺϳ�,�ο���Ӧ�ļ����ͼ��ѧ�鼮,   
#     //�任�ľ��������ο�:   
#     //<�����ͼ��ѧ>(������),��ҹ�,�廪��ѧ������   
#        
#     //���������Ľ��   
#     float xx = (fx * fx * (1.0f - c) + c)      * x +   
#                (fx * fy * (1.0f - c) - fz * s) * y +   
#                (fx * fz * (1.0f - c) + fy * s) * z;   
#    
#     float yy = (fy * fx * (1.0f - c) + fz * s) * x +   
#                (fy * fy * (1.0f - c) + c)      * y +   
#                (fy * fz * (1.0f - c) - fx * s) * z;   
#    
#     float zz = (fz * fx * (1.0f - c) - fy * s) * x +    
#                (fz * fy * (1.0f - c) + fx * s) * y +   
#                (fz * fz * (1.0f - c) + c)      * z;   
#    
#     //���������   
#     x = xx;   
#     y = yy;   
#     z = zz;   
# }   
#    
# //����������ת, fThetaΪ�Ƕ�,    
# //����ָ��, �������Ϊһ����������, �������������Դ�����ԭ�㻭��   
# void CVector3d::Rotate(float fTheta,  VECTOR3D axis)   
# {   
#     CVector3d vector = axis;   
#     this->Rotate(fTheta, vector);   
# }   
#    
# //(x, y, z)Ϊ��ת������   
# void CVector3d::Rotate(float fTheta,  float x, float y, float z)   
# {   
#     CVector3d vector(x, y, z);   
#     this->Rotate(fTheta, vector);   
# }   
#    
# //////////////////////////////////////////////////////////////////////////////////////////////   
# //   
# //                    ����С�淨��   
# //   
# //////////////////////////////////////////////////////////////////////////////////////////////   
#    
#    
# //����С�淨��   
# VECTOR3D CVector3d::Noraml(const VERTEX3D& v1,  const VERTEX3D& v2,  const VERTEX3D& v3)   
# {   
#     //���ֹ���   
#     CVector3d vector1((v2.x - v1.x),  (v2.y - v1.y),  (v2.z - v1.z));    
#     CVector3d vector2((v3.x - v1.x),  (v3.y - v1.y),  (v3.z - v1.z));    
#     CVector3d noraml = vector1 * vector2;   
#     VECTOR3D n = {noraml.x,  noraml.y,  noraml.z};   
#     return n;   
# }    
#    
# //����С�淨��   
# VECTOR3D CVector3d::Noraml(const HOMOCOORD& v1,  const HOMOCOORD& v2,  const HOMOCOORD& v3)   
# {   
#     //���һ���������а�������Զ��, �����������һ������������,    
#     float w1 = v1.w,  w2 = v2.w,  w3 = v3.w;   
#     if(ABS(w1) <= 0.00001f) w1 = 1.0f;   
#     if(ABS(w2) <= 0.00001f) w2 = 1.0f;   
#     if(ABS(w3) <= 0.00001f) w3 = 1.0f;   
#    
#     float x1 = v1.x / w1,  y1 = v1.y / w1,  z1 = v1.z / w1;   
#     float x2 = v2.x / w2,  y2 = v2.y / w2,  z2 = v2.z / w2;   
#     float x3 = v3.x / w3,  y3 = v3.y / w3,  z3 = v3.z / w3;   
#    
#     CVector3d vector1((x2 - x1),  (y2 - y1),  (z2 - z1));    
#     CVector3d vector2((x3 - x1),  (y3 - y1),  (z3 - z1));    
#     CVector3d noraml = vector1 * vector2;   
#     VECTOR3D n = {noraml.x,  noraml.y,  noraml.z};   
#     return n;   
# }