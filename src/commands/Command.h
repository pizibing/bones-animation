#pragma once

class Calculator;
class Display;
class MeshManager;
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
	MeshManager* meshManager;
	CameraManager* cameraManager;
	LightManager* lightManager;
};
