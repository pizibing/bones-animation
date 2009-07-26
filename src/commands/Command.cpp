#include "Command.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../matrixlib/matrix.h"
#include "../data/MoveSelfObject.h"
#include "../data/character/CharacterObject.h"
#include "../view/Display.h"
#include "../view/ConcreteDisplay.h"
#include "../managers/CameraManager.h"
#include "../managers/ObjectManager.h"
#include "../managers/LightManager.h"
#include "../managers/DisplayManager.h"
#include "../calculation/Calculator.h"
#include "../loaders/ModelLoader.h"
#include "../loaders/CommandModelLoader.h"
#include <gl/glut.h>

//constructor
Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = CommandModelLoader::getInstance();
	calculator = new Calculator();
	objectManager = ObjectManager::getInstance();
	cameraManager = CameraManager::getInstance();
	lightManager = LightManager::getInstance();
	displayManager = new DisplayManager();
}

//destructor
Command::~Command(void){
	delete display;
	delete calculator;
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

// draw the character object of this scene in skeleton lines
void Command::drawLineCharacter(){
	// camera
	cameraManager->look();
	// get character
	CharacterObject* character = (CharacterObject*)displayManager->getDisplayedCharacter(0);
	// get lines of that character
	int num = 0;
	SimpleLine* lines = character->representInLine(&num);
	// display
	display->display(lines,num);
}

//load all default models
bool Command::loadModel(){
	modelLoader->loadModel(TERRAIN_LOADER_KIND,"resource/heightMap/heightmap.bmp");
	return modelLoader->loadModel(CHARACTER_LOADER_KIND,"resource/run_niki.dae");
}

// load a model with the given path
bool Command::loadModel(const char* path){
	return modelLoader->loadModel(0,path);
}

// rotate camera in a plain that is vertical to +z axis and interact
// at z = CAMERA_HEIGHT + characterPostion.
// angle should be presented in degree
// if angle is positive, camera move clock wise
// vice versa
void Command::rotateCamera(float angle){
	// use Camera Manager to implement this function
	cameraManager->rotateCamera(angle);
}

// character move forward in the given direction
// direction can be ARROW_NONE,ARROW_UP,ARROW_DOWN
// ARROW_LEFT,ARROW_RIGHT.which none means no moveforward
// up,down,left and right is always the direction on the screen
void Command::moveCharacter(int direction){
	// calculate the direction angle according to camera angle
	// and the arrow direction given
	float angle = 0;
	switch(direction){
		// if arrow none, slow down
		case ARROW_NONE:
			angle = -1;
			break;
		case ARROW_UP:
			angle = 180 + cameraManager->getAngle();
			break;
		case ARROW_DOWN:
			angle = cameraManager->getAngle();
			break;
		case ARROW_LEFT:
			angle = 270 + cameraManager->getAngle();
			break;
		case ARROW_RIGHT:
			angle = 90 + cameraManager->getAngle();
			break;
		case ARROW_LEFT_UP:
			angle = 225 + cameraManager->getAngle();
			break;
		case ARROW_RIGHT_UP:
			angle = 135 + cameraManager->getAngle();
			break;
		case ARROW_RIGHT_DOWN:
			angle = 45 + cameraManager->getAngle();
			break;
		case ARROW_LEFT_DOWN:
			angle = 315 + cameraManager->getAngle();
			break;
		default:
			angle = -1;
			break;
	}

	// modify angle to fit into -1 to 360
	if(angle > 0){
		// get int divide result 
		int r = (int)(angle / 360);
		// get the remain
		angle = angle - 360 * r;
	}

	// use calculator to calculate character move
	calculator->moveCharacter(angle);
}