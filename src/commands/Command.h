#pragma once

class Calculator;
class Display;
class ObjectManager;
class ModelLoader;
class CameraManager;
class LightManager;
class DisplayManager;

#ifndef _DIRECTION_ARROW
#define _DIRECTION_ARROW
// define direction possible value
const int ARROW_NONE = 0;
const int ARROW_UP = 1;
const int ARROW_DOWN = 2;
const int ARROW_LEFT = 3;
const int ARROW_RIGHT = 4;
#endif

// Command is used as the conning tower of this system
// it is used by main.cpp to do tasks
// it complishes all the tasks by commanding the managers
class Command
{
public:
	Command(void);
	~Command(void);

	// draw all the objects that should be displayed in the
	// scene right now
	void drawScene();

	//load all default models
	bool loadModel();

	// load a model with the given path
	bool loadModel(const char* path);

	// rotate camera in a plain that is vertical to +z axis and interact
	// at z = CAMERA_HEIGHT + characterPostion.
	// angle should be presented in degree
	// if angle is positive, camera move clock wise
	// vice versa
	void rotateCamera(float angle);

	// character move forward in the given direction
	// direction can be ARROW_NONE,ARROW_UP,ARROW_DOWN
	// ARROW_LEFT,ARROW_RIGHT.which none means no moveforward
	// up,down,left and right is always the direction on the screen
	void moveCharacter(int direction);

private:
	// all managers
	Display* display;
	ModelLoader* modelLoader;
	Calculator* calculator;
	ObjectManager* objectManager;
	CameraManager* cameraManager;
	LightManager* lightManager;
	DisplayManager* displayManager;
};
