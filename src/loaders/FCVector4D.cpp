#include "FCVector4D.h"


FCVector4D::FCVector4D(float x, float y, float z, float w)
{
	m_x=x;
	m_y=y;
	m_z=z;
	m_w=w;
}

FCVector4D::FCVector4D()
{
	m_x=0;
	m_y=0;
	m_z=0;
	m_w=0;
}

FCVector4D::~FCVector4D(void)
{

}

float FCVector4D::GetX(void)
{
	return m_x;
}

float FCVector4D::GetY(void)
{
	return m_y;
}

float FCVector4D::GetZ(void)
{
	return m_z;
}

float FCVector4D::GetW(void)
{
	return m_w;
}

void FCVector4D::SetX(float x)
{
	m_x=x;
}

void FCVector4D::SetY(float y)
{
	m_y=y;
}

void FCVector4D::SetZ(float z)
{
	m_z=z;
}

void FCVector4D::SetW(float w)
{
	m_w=w;
}

void FCVector4D::Set(float x, float y, float z, float w) 
{
	m_x=x;
	m_y=y;
	m_z=z;
	m_w=w;
}