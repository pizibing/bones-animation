#include <math.h>
#include <assert.h>
#include "CameraManager.h"

CameraManager* CameraManager::singleton = 0;

// get the single instance of CameraManager
// if CameraManager is never used before, a new one will be created
CameraManager* CameraManager::getInstance(){
	// if singleton is null, create a new one
	if(!singleton){
		singleton = new CameraManager();
	}

	return singleton;
}

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

	// init camera height and distance
	CAMERA_HEIGHT = 5;
	CAMERA_DISTANCE = 5;
}

// destructor
CameraManager::~CameraManager(void){
}

// rotate camera in a plain that is vertical to +z axis and interact
// at z = CAMERA_HEIGHT + characterPostion.
// angle should be presented in degree
// if angle is positive, camera move clock wise
// vice versa
void CameraManager::rotateCamera(float angle){
	// change this-> angle according to the given angle
	this->angle += angle;
	// if angle is bigger than 0, just get the remain
	if(this->angle >= 0){
		// get int divide result 
		int r = (int)(this->angle / 360);
		// get the remain
		this->angle = this->angle - 360 * r;
	}
	// if angle is smaller than 0
	else{
		// get int divide result 
		int r = (int)(this->angle / 360);
		// get the remain
		this->angle = this->angle - 360 * r;
		// check angle
		assert(this->angle <= 0);
		assert(this->angle > -360);
		// 2 pi minus angle
		this->angle = 360 + this->angle;
	}

	// the result should be between 0 and 360
	assert(this->angle >= 0);
	assert(this->angle < 360);
}

// set the target position of the camera
void CameraManager::setTarget(float tx, float ty, float tz){

	// set target
	target[0] = tx;
	target[1] = ty;
	target[2] = tz;
}

// use glLookAt() to update the camera
void CameraManager::look(){
	// calculate position according to target and angle
	position[0] = CAMERA_DISTANCE * cos(angle / 180 * PI) + target[0];
	position[1] = CAMERA_DISTANCE * sin(angle / 180 * PI) + target[1];
	position[2] = CAMERA_HEIGHT + target[2];
	// use gluLookAt to set camera
	gluLookAt(position[0],position[1],position[2],
		      target[0]  ,target[1]  ,target[2]  ,
			  gesture[0] ,gesture[1] ,gesture[2]  );
}

// get function of angle
float CameraManager::getAngle(){
	return angle;
}

// move camera closer to the character
void CameraManager::moveCloser(){
	// camera should not be to closer
	if(CAMERA_HEIGHT <= 0.3 || CAMERA_DISTANCE <= 0.3) return;
	CAMERA_HEIGHT = CAMERA_HEIGHT - 0.1;
	CAMERA_DISTANCE = CAMERA_DISTANCE - 0.1;
}

// move camera away from the character
void CameraManager::moveAway(){
	// camera should not be to far away
	if(CAMERA_HEIGHT >= 5 || CAMERA_DISTANCE >= 5) return;
	CAMERA_HEIGHT = CAMERA_HEIGHT + 0.1;
	CAMERA_DISTANCE = CAMERA_DISTANCE + 0.1;
}