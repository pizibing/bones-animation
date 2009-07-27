#pragma once

#include <gl/glut.h>

// the distance from the character to the camera in vertical(+z axis)
const float CAMERA_HEIGHT = 5;
// the distance from the character to the camera in horizontal
const float CAMERA_DISTANCE = 5;

// PI
const float PI = 3.1415;

// CameraManager manages the camera in openGL
// you can use this class to change the position and
// posture of the camera without touching the complicate
// openGL camera control.
class CameraManager
{
public:
	// get the single instance of CameraManager
	// if CameraManager is never used before, a new one will be created
	static CameraManager* getInstance();

	// rotate camera in a plain that is vertical to +z axis and interact
	// at z = CAMERA_HEIGHT + characterPostion.
	// angle should be presented in degree
	// if angle is positive, camera move clock wise
	// vice versa
	void rotateCamera(float angle);

	// set the target position of the camera
	void setTarget(float tx, float ty, float tz);

	// use glLookAt() to update the camera
	void look();

	// get function of angle
	float getAngle();

private:
	// constructor
	CameraManager(void);
	// destructor
	~CameraManager(void);

	// single instance of CameraManager
	static CameraManager* singleton;

	// the position of the camera
	float position[3];
	// the point the camera look at
	float target[3];
	// the gesture of the camera
	float gesture[3];
	// the angle of the camera position to the +x axis in horizontal
	// angle is presented in degree, 0 to 360 is allowed
	float angle;
};
