#include "Vector4D.h"


Vector4D::Vector4D(float vx, float vy, float vz, float vw)
: x(vx),y(vy),z(vz),w(vw)
{}

Vector4D::Vector4D()
: x(0.0f),y(0.0f),z(0.0f),w(0.0f)
{
}

void Vector4D::Set(float vx, float vy, float vz, float vw) 
{
	x=vx;
	y=vy;
	z=vz;
	w=vw;
}