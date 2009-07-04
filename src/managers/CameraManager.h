#pragma once

#include <gl/glut.h>

// CameraManager manages the camera in openGL
// you can use this class to change the position and
// posture of the camera without touching the complicate
// openGL camera control.
class CameraManager
{
public:
	// constructor
	CameraManager(void);
	// destructor
	~CameraManager(void);

	// use glLookAt() to update the camera
	void look();
};
