#pragma once

class ModelLoader;
class ObjectManager;

class SimpleModelLoader :
	public ModelLoader
{
public:
	SimpleModelLoader(void);
	~SimpleModelLoader(void);

	bool loadModel(int kind, char* path);

private:
	ObjectManager* objectManager;
};
