#pragma once
#include "modelloader.h"
#include "../managers/MeshManager.h"

class SimpleModelLoader :
	public ModelLoader
{
public:
	SimpleModelLoader(void);
	~SimpleModelLoader(void);

	bool loadModel(int kind, char* path);

private:
	MeshManager* meshManager;
};
