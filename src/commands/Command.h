#pragma once

class Calculator;
class Display;
class ObjectManager;
class ModelLoader;
class CameraManager;
class LightManager;
class DisplayManager;

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
	// angle should be presented in rad
	// if angle is positive, camera move clock wise
	// vice versa
	void rotateCamera(float angle);

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
