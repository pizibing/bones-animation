#pragma once

#include <gl/glut.h>

class CameraManager
{
public:
	CameraManager(void);
	~CameraManager(void);

	//use glLookAt() to look
	void look();
};
