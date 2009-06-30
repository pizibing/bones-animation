#include "CameraManager.h"

CameraManager::CameraManager(void){
}

CameraManager::~CameraManager(void){
}

void CameraManager::look(){
	gluLookAt(10,8,6,0,0,0,0,1,0);
}
