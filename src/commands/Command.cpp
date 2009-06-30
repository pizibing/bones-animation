#include "Command.h"
#include "../view/ConcreteDisplay.h"
#include "../loaders/SimpleModelLoader.h"
#include "../managers/SimpleMeshManager.h"
#include <gl/glut.h>

Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = new SimpleModelLoader();
	calculator = new Calculator();
	meshManager = SimpleMeshManager::getInstance();
	cameraManager = new CameraManager();
	lightManager = LightManager::getInstance();
}

Command::~Command(void){
	delete display;
	delete modelLoader;
	delete calculator;
	delete cameraManager;
}

void Command::drawScene(){
	//calculate
	calculator->rotate(1,AXIS_X);
	//light
	GLfloat* position = lightManager->getPosition(0);
	position[1] = 6;
	glEnable(GL_LIGHT0);
	//draw
	cameraManager->look();
	display->display(false,meshManager->getMeshes(MESH_KIND_CHARACTER));
}

bool Command::loadModel(){
	return modelLoader->loadModel(0,"");
}
