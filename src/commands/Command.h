#pragma once

class Calculator;
class Display;
class ObjectManager;
class ModelLoader;
class CameraManager;
class LightManager;

class Command
{
public:
	Command(void);
	~Command(void);

	//draw all the meshes in the mesh manager into world space
	void drawScene();

	//load all default models
	bool loadModel();

private:
	Display* display;
	ModelLoader* modelLoader;
	Calculator* calculator;
	ObjectManager* objectManager;
	CameraManager* cameraManager;
	LightManager* lightManager;
};
