#include "TerrainModelLoader.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/terrain/TerrainObject.h"
#include "../managers/ObjectManager.h"

// constructor
TerrainModelLoader::TerrainModelLoader(void){
}

// destructor
TerrainModelLoader::~TerrainModelLoader(void){
}

/*
* para: path: load a model with the given path
*       kind: specifies the role of the model in the entire world(e.g terrain,character etc.)
* function: load the model with the given path and set it in the ObjectManager
* return: true when model is successfully loaded, vice versa
*/
bool TerrainModelLoader::loadModel(int kind, const char* path){
	// create a new TerrainObject
	TerrainObject* terrain = new TerrainObject();

	// get ObjectManager
	ObjectManager* om = ObjectManager::getInstance();
	// add terrain into ObjectManager
	om->addVBOObject(terrain);
	return true;
}
