#pragma once

class ObjectManager;

class SimpleModelLoader 
{
public:
	SimpleModelLoader(void);
	~SimpleModelLoader(void);

	bool loadModel(int kind, char* path);

private:
	ObjectManager* objectManager;
};
