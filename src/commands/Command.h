#pragma once

#include "../calculation/Calculator.h"
#include "../view/Display.h"
#include "../managers/MeshManager.h"
#include "../loaders/ModelLoader.h"

class Command
{
public:
	Command(void);
	~Command(void);

	//draw all the meshes in the mesh manager into world space
	void drawScene();

private:
	Display* display;
	ModelLoader* modelLoader;
	Calculator* calculator;
	MeshManager* meshManager;
};
