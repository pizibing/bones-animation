#pragma once

#include "Caculator.h"
#include "Display.h"
#include "MeshManager.h"
#include "ModelLoader.h"

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
	Caculator* caculator;
	MeshManager* meshManager;
};
