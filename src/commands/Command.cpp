#include "Command.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../view/Display.h"
#include "../view/ConcreteDisplay.h"
#include "../managers/CameraManager.h"
#include "../managers/ObjectManager.h"
#include "../managers/LightManager.h"
#include "../managers/DisplayManager.h"
#include "../calculation/Calculator.h"
#include "../loaders/ModelLoader.h"
#include "../loaders/FColladaModelLoader.h"
#include <gl/glut.h>

//constructor
Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = new FColladaModelLoader();
	calculator = new Calculator();
	objectManager = ObjectManager::getInstance();
	cameraManager = new CameraManager();
	lightManager = LightManager::getInstance();
	displayManager = new DisplayManager();
}

//destructor
Command::~Command(void){
	delete display;
	delete modelLoader;
	delete calculator;
	delete cameraManager;
}

// draw all the objects that should be displayed in the
// scene right now
void Command::drawScene(){
	//calculate
	//calculator->rotate(1,AXIS_X);
	//light
	GLfloat* position = lightManager->getPosition(0);
	position[1] = 6;
	glEnable(GL_LIGHT0);
	//draw
	cameraManager->look();
	// get objects to display
	std::vector<VBOObject*>* objects = displayManager->getDisplayedObjectsAll();
	// draw each object
	for(int i = 0; i < (signed int)objects->size(); i++){
		VBOObject* vboobject = (*objects)[i];
		int num = 0;
		VBOMesh* vbomesh = vboobject->representInVBOMesh(&num);
		display->display(false, vbomesh, num); 
	}
	delete objects;
	/*VBOObject* vboobject = objectManager->getVBOObject(2,4);
	if(!vboobject) return;
	int num = 0;
	VBOMesh* vbomesh = vboobject->representInVBOMesh(&num);
	display->display(false, vbomesh, num); */
}

//load all default models
bool Command::loadModel(){
	//modelLoader->loadModel(0,"cube.dae");
	return modelLoader->loadModel(2,"niki.dae");
}

// load a model with the given path
bool Command::loadModel(const char* path){
	return modelLoader->loadModel(0,path);
}

// rotate camera in a plain that is vertical to +z axis and interact
// at z = CAMERA_HEIGHT + characterPostion.
// angle should be presented in rad
// if angle is positive, camera move clock wise
// vice versa
void Command::rotateCamera(float angle){
	// use Camera Manager to implement this function
	cameraManager->rotateCamera(angle);
}