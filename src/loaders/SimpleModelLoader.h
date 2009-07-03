#pragma once

class ModelLoader;
class MeshManager;

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
