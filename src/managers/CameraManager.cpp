#include "CameraManager.h"

CameraManager::CameraManager(void){
}

CameraManager::~CameraManager(void){
}

// use glLookAt() to update the camera
void CameraManager::look(){
	gluLookAt(100,0,30,0,0,0,0,0,1);
}
