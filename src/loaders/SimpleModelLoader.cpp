#include "SimpleModelLoader.h"
#include "../managers/SimpleMeshManager.h"
#include "../managers/TextureManager.h"

SimpleModelLoader::SimpleModelLoader(void){
}

SimpleModelLoader::~SimpleModelLoader(void){
}

bool SimpleModelLoader::loadModel(int kind, char* path){
	meshManager = SimpleMeshManager::getInstance();

	//texture
	TextureManager* textureManager = TextureManager::getInstance();
	GLuint tex1 = textureManager->getTextureId("resource/leaf2.jpg");
	GLuint tex2 = textureManager->getTextureId("resource/Reflection.jpg");

	Mesh* mesh = new Mesh();
	//upper face
	Polygon* polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,0.5));
	polygon->hasTexture = true;
	polygon->texID = tex1;
	polygon->verticeTexCoords->push_back(new Vector3D(0,0,0));
	polygon->verticeTexCoords->push_back(new Vector3D(0,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,0,0));
	polygon->hasNormals = true;
	polygon->verticeNormals->push_back(new Vector3D(0,1,0));
	polygon->verticeNormals->push_back(new Vector3D(0,1,0));
	polygon->verticeNormals->push_back(new Vector3D(0,1,0));
	polygon->verticeNormals->push_back(new Vector3D(0,1,0));
	mesh->polygons->push_back(polygon);
	//down face
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,0.5));
	polygon->hasTexture = true;
	polygon->texID = tex1;
	polygon->verticeTexCoords->push_back(new Vector3D(0,0,0));
	polygon->verticeTexCoords->push_back(new Vector3D(0,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,0,0));
	polygon->hasNormals = true;
	polygon->verticeNormals->push_back(new Vector3D(0,-1,0));
	polygon->verticeNormals->push_back(new Vector3D(0,-1,0));
	polygon->verticeNormals->push_back(new Vector3D(0,-1,0));
	polygon->verticeNormals->push_back(new Vector3D(0,-1,0));
	mesh->polygons->push_back(polygon);
	//left
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,0.5));
	polygon->hasTexture = true;
	polygon->texID = tex1;
	polygon->verticeTexCoords->push_back(new Vector3D(0,0,0));
	polygon->verticeTexCoords->push_back(new Vector3D(0,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,0,0));
	polygon->hasNormals = true;
	polygon->verticeNormals->push_back(new Vector3D(-1,0,0));
	polygon->verticeNormals->push_back(new Vector3D(-1,0,0));
	polygon->verticeNormals->push_back(new Vector3D(-1,0,0));
	polygon->verticeNormals->push_back(new Vector3D(-1,0,0));
	mesh->polygons->push_back(polygon);
	//right
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,0.5));
	polygon->hasTexture = true;
	polygon->texID = tex1;
	polygon->verticeTexCoords->push_back(new Vector3D(0,0,0));
	polygon->verticeTexCoords->push_back(new Vector3D(0,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,0,0));
	polygon->hasNormals = true;
	polygon->verticeNormals->push_back(new Vector3D(1,0,0));
	polygon->verticeNormals->push_back(new Vector3D(1,0,0));
	polygon->verticeNormals->push_back(new Vector3D(1,0,0));
	polygon->verticeNormals->push_back(new Vector3D(1,0,0));
	mesh->polygons->push_back(polygon);
	//front
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,0.5));
	polygon->hasTexture = true;
	polygon->texID = tex1;
	polygon->verticeTexCoords->push_back(new Vector3D(0,0,0));
	polygon->verticeTexCoords->push_back(new Vector3D(0,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,0,0));
	polygon->hasNormals = true;
	polygon->verticeNormals->push_back(new Vector3D(0,0,1));
	polygon->verticeNormals->push_back(new Vector3D(0,0,1));
	polygon->verticeNormals->push_back(new Vector3D(0,0,1));
	polygon->verticeNormals->push_back(new Vector3D(0,0,1));
	mesh->polygons->push_back(polygon);
	//back
	polygon = new Polygon();
	polygon->vertices->push_back(new Vector3D(0.5,0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,-0.5,-0.5));
	polygon->vertices->push_back(new Vector3D(-0.5,0.5,-0.5));
	polygon->hasTexture = true;
	polygon->texID = tex1;
	polygon->verticeTexCoords->push_back(new Vector3D(0,0,0));
	polygon->verticeTexCoords->push_back(new Vector3D(0,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,1,0));
	polygon->verticeTexCoords->push_back(new Vector3D(1,0,0));
	polygon->hasNormals = true;
	polygon->verticeNormals->push_back(new Vector3D(0,0,-1));
	polygon->verticeNormals->push_back(new Vector3D(0,0,-1));
	polygon->verticeNormals->push_back(new Vector3D(0,0,-1));
	polygon->verticeNormals->push_back(new Vector3D(0,0,-1));
	mesh->polygons->push_back(polygon);

	meshManager->addMesh(0,mesh);

	return true;
}
