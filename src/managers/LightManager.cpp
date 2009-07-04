#include "LightManager.h"
#include <string>

// initialize the singleton to NULL
LightManager* LightManager::singleton = NULL;

// get the single instance of LightManager, if LightManager
// is never used before, create a new instance
LightManager* LightManager::getInstance(){
	if(singleton == NULL){
		singleton = new LightManager();
	}
	return singleton;
}

// constructor
// initialize all the information of light to the same as
// default in openGL
LightManager::LightManager(void){
	for(int i = 0; i < LIGHT_NUM; i++){
		ambient[i][0] = 0;ambient[i][1] = 0;ambient[i][2] = 0;ambient[i][3] = 1;
		diffuse[i][0] = 1;diffuse[i][1] = 1;diffuse[i][2] = 1;diffuse[i][3] = 1;
		specular[i][0] = 1;specular[i][1] = 1;specular[i][2] = 1;specular[i][3] = 1;
		position[i][0] = 0;position[i][1] = 0;position[i][2] = 1;position[i][3] = 0;
	}
}

//destructor
LightManager::~LightManager(void){
}

// get the ambient of the i-th light
GLfloat* LightManager::getAmbient(int i){
	return ambient[i];
}

// get the diffuse of the i-th light
GLfloat* LightManager::getDiffuse(int i){
	return diffuse[i];
}

// get the specular of the i-th light
GLfloat* LightManager::getSpecular(int i){
	return specular[i];
}

// get the position of the i-th light
GLfloat* LightManager::getPosition(int i){
	return position[i];
}

