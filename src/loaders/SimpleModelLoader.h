#pragma once
#include "modelloader.h"

class SimpleModelLoader :
	public ModelLoader
{
public:
	SimpleModelLoader(void);
	~SimpleModelLoader(void);

	bool loadModel(int kind, char* path);
};
