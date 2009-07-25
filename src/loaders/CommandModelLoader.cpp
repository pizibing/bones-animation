#include <olectl.h>	
#include <assert.h>
#include <string>
#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "../data/VBOMesh.h"
#include "../data/SimpleLine.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../data/MoveSelfObject.h"
#include "../data/static/StaticObject.h"
#include "../data/character/ChVertex.h"
#include "../data/character/ChSkin.h"
#include "../data/character/ChBone.h"
#include "../data/character/ChSkeleton.h"
#include "../data/character/ChKeyFrame.h"
#include "../data/character/ChTrack.h"
#include "../data/character/ChAnimation.h"
#include "../data/character/ChAnimationManager.h"
#include "../data/character/CharacterObject.h"
#include "../managers/ObjectManager.h"
#include "ModelLoader.h"
#include "../managers/TextureManager.h"
#include "FCVector4D.h"
#include "ModelLoader.h"
#include "CommandModelLoader.h"
#include "FColladaModelLoader.h"
#include "TerrainModelLoader.h"

// initialize singleton to null
CommandModelLoader* CommandModelLoader::singleton = 0;

// constructor
CommandModelLoader::CommandModelLoader(void){
	// initialize three kind of loader
	floader = new FColladaModelLoader();
	tloader = new TerrainModelLoader();
}

// destructor
CommandModelLoader::~CommandModelLoader(void){
	delete floader;
	delete tloader;
}

// get the single instance of CommandModelLoader
CommandModelLoader* CommandModelLoader::getInstance(){
	// if singleton is null , create a new one
	if(!singleton){
		singleton = new CommandModelLoader();
	}

	return singleton;
}

/*
* para: path: load a model with the given path
*       kind: specifies the role of the model in the entire world(e.g terrain,character etc.)
* function: load the model with the given path and set it in the ObjectManager
* return: true when model is successfully loaded, vice versa
*/
bool CommandModelLoader::loadModel(int kind, const char* path){
	// according to kind, arrange corresponding ModelLoader to
	// achieve the task
	switch(kind){
		case CHARACTER_LOADER_KIND:
			return floader->loadModel(kind,path);
			break;
		case TERRAIN_LOADER_KIND:
			return tloader->loadModel(kind,path);
			break;
		case STATIC_LOADER_KIND:
			return floader->loadModel(kind,path);
			break;
		default:
			return false;
	}
}

