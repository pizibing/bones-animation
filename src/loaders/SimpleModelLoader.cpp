#include "SimpleModelLoader.h"
#include "../managers/SimpleMeshManager.h"

SimpleModelLoader::SimpleModelLoader(void){
}

SimpleModelLoader::~SimpleModelLoader(void){
}

bool SimpleModelLoader::loadModel(int kind, char* path){
	meshManager = SimpleMeshManager::getInstance();

	Mesh* mesh = new Mesh();
	//upper face
	Polygon* polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,0.5));
	mesh->polygons->push_back(polygon);
	//down face
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,0.5));
	mesh->polygons->push_back(polygon);
	//left
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,0.5));
	mesh->polygons->push_back(polygon);
	//right
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,0.5));
	mesh->polygons->push_back(polygon);
	//front
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,0.5));
	mesh->polygons->push_back(polygon);
	//back
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,-0.5));
	mesh->polygons->push_back(polygon);

	meshManager->addMesh(0,mesh);

	return true;
}
