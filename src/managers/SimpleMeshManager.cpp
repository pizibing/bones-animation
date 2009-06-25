#include "SimpleMeshManager.h"

SimpleMeshManager* SimpleMeshManager::singleton = NULL;

SimpleMeshManager::SimpleMeshManager(void){
	meshes = new std::vector<std::vector<Mesh*>*>();
	for(int i = 0; i < MESH_KIND_NUM; i++){
		std::vector<Mesh*>* mesh = new std::vector<Mesh*>();
		meshes->push_back(mesh);
	}
}

SimpleMeshManager::~SimpleMeshManager(void){
}

SimpleMeshManager* SimpleMeshManager::getInstance(){
	if(singleton == NULL)
		singleton = new SimpleMeshManager();
	return singleton;
}

void SimpleMeshManager::addMesh(int kind, Mesh* newMesh){
	std::vector<Mesh*>* mesh = (*meshes)[kind];
	mesh->push_back(newMesh);
}

bool SimpleMeshManager::deleteMesh(int kind, int i){
	return false;
}

Mesh* SimpleMeshManager::getMesh(int kind, int i){
	std::vector<Mesh*>* mesh = (*meshes)[kind];
	return (*mesh)[i];
}

std::vector<Mesh*>* SimpleMeshManager::getMeshes(int kind){
	return (*meshes)[kind];
}
