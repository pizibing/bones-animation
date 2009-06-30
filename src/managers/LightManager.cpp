#include "LightManager.h"
#include <string>

LightManager* LightManager::singleton = NULL;

LightManager* LightManager::getInstance(){
	if(singleton == NULL){
		singleton = new LightManager();
	}
	return singleton;
}

LightManager::LightManager(void){
	for(int i = 0; i < LIGHT_NUM; i++){
		ambient[i][0] = 0;ambient[i][1] = 0;ambient[i][2] = 0;ambient[i][3] = 1;
		diffuse[i][0] = 1;diffuse[i][1] = 1;diffuse[i][2] = 1;diffuse[i][3] = 1;
		specular[i][0] = 1;specular[i][1] = 1;specular[i][2] = 1;specular[i][3] = 1;
		position[i][0] = 0;position[i][1] = 0;position[i][2] = 1;position[i][3] = 0;
	}
}

LightManager::~LightManager(void)
{
}

GLfloat* LightManager::getAmbient(int i){
	return ambient[i];
}

GLfloat* LightManager::getDiffuse(int i){
	return diffuse[i];
}

GLfloat* LightManager::getSpecular(int i){
	return specular[i];
}

GLfloat* LightManager::getPosition(int i){
	return position[i];
}

