#include "CameraManager.h"

CameraManager::CameraManager(void){
}

CameraManager::~CameraManager(void){
}

// use glLookAt() to update the camera
void CameraManager::look(){
	gluLookAt(3,4,5,0,0,0,0,0,1);
}
