#include "Command.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../view/Display.h"
#include "../view/ConcreteDisplay.h"
#include "../managers/CameraManager.h"
#include "../managers/ObjectManager.h"
#include "../managers/LightManager.h"
#include "../calculation/Calculator.h"
#include "../loaders/ModelLoader.h"
#include "../loaders/FColladaModelLoader.h"
#include <gl/glut.h>

Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = new FColladaModelLoader();
	calculator = new Calculator();
	objectManager = ObjectManager::getInstance();
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
	//calculator->rotate(1,AXIS_X);
	//light
	GLfloat* position = lightManager->getPosition(0);
	position[1] = 6;
	glEnable(GL_LIGHT0);
	//draw
	cameraManager->look();
	//display->display(false,meshManager->getMeshes(MESH_KIND_CHARACTER));
	//test for vbomesh
	/*GLfloat vertices[] = {0.5,0.5,0.5,0.5,0.5,-0.5,-0.5,0.5,-0.5};
	VBOMesh* vbomesh = new VBOMesh(vertices,9,GL_DYNAMIC_DRAW);
	display->display(false,vbomesh,1);*/
	VBOObject* vboobject = objectManager->getVBOObject(2, 1);
	int num = 0;
	VBOMesh* vbomesh = vboobject->representInVBOMesh(&num);
	display->display(false, vbomesh, num); 
}

bool Command::loadModel(){
	return modelLoader->loadModel(0,"knight.dae");
}
