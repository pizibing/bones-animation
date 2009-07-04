#pragma once

class Calculator;
class Display;
class ObjectManager;
class ModelLoader;
class CameraManager;
class LightManager;

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

private:
	// all managers
	Display* display;
	ModelLoader* modelLoader;
	Calculator* calculator;
	ObjectManager* objectManager;
	CameraManager* cameraManager;
	LightManager* lightManager;
};
