#include <math.h>
#include <assert.h>
#include "CameraManager.h"

// constructor
CameraManager::CameraManager(void){
	// initialize angle
	angle = 0;
	// initialize position, target and gesture
	position[0] = CAMERA_DISTANCE * cos(angle);
	position[1] = CAMERA_DISTANCE * sin(angle);
	position[2] = CAMERA_HEIGHT;

	target[0] = 0;
	target[1] = 0;
	target[2] = 0;

	gesture[0] = 0;
	gesture[1] = 0;
	gesture[2] = 1;
}

// destructor
CameraManager::~CameraManager(void){
}

// rotate camera in a plain that is vertical to +z axis and interact
// at z = CAMERA_HEIGHT + characterPostion.
// angle should be presented in rad
// if angle is positive, camera move clock wise
// vice versa
void CameraManager::rotateCamera(float angle){
	// change this-> angle according to the given angle
	this->angle += angle;
	// if angle is bigger than 0, just get the remain
	if(this->angle > 0){
		// get int divide result 
		int r = (int)(this->angle / (2 * PI));
		// get the remain
		this->angle = this->angle - 2 * PI * r;
	}
	// if angle is smaller than 0
	else{
		// get int divide result 
		int r = (int)(this->angle / (2 * PI));
		// get the remain
		this->angle = this->angle - 2 * PI * r;
		// check angle
		assert(angle < 0);
		assert(angle > -(2 * PI));
		// 2 pi minus angle
		this->angle = 2 * PI + angle;
	}
}

// set the target position of the camera
void CameraManager::setTarget(float* tp){
	// tp should not be null
	assert(tp);

	// set target
	for(int i = 0; i < 3; i++)
		target[i] = tp[i];
}

// use glLookAt() to update the camera
void CameraManager::look(){
	// calculate position according to target and angle
	position[0] = CAMERA_DISTANCE * cos(angle) + target[0];
	position[1] = CAMERA_DISTANCE * sin(angle) + target[1];
	position[2] = CAMERA_HEIGHT + target[2];
	// use gluLookAt to set camera
	gluLookAt(position[0],position[1],position[2],
		      target[0]  ,target[1]  ,target[2]  ,
			  gesture[0] ,gesture[1] ,gesture[2]  );
}
