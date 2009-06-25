#pragma once
#include "meshmanager.h"

class SimpleMeshManager :
	public MeshManager
{
public:
	SimpleMeshManager(void);
	~SimpleMeshManager(void);


	void addMesh(int kind, Mesh* newMesh);
	bool deleteMesh(int kind, int i);
	Mesh* getMesh(int kind, int i);
	std::vector<Mesh*> getMeshes(int kind);
};
