#pragma once
#include "meshmanager.h"

class SimpleMeshManager :
	public MeshManager
{
public:
	static SimpleMeshManager* getInstance();

	void addMesh(int kind, Mesh* newMesh);
	bool deleteMesh(int kind, int i);
	Mesh* getMesh(int kind, int i);
	std::vector<Mesh*>* getMeshes(int kind);

private:
	SimpleMeshManager(void);
	~SimpleMeshManager(void);

	static SimpleMeshManager* singleton;

	std::vector<std::vector<Mesh*>*>* meshes;
};
