#include "imageloader.h"
#include "TerrainModelLoader.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/terrain/TerrainObject.h"
#include "../managers/ObjectManager.h"
#include "../managers/TextureManager.h"

// define the scale from file read height to real display height
const float HEIGHT_SCALE = 0.015;
// define stride
const float STRIDE = 0.5;

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
	// load height information from image file
	Image* image = loadBMP(path);
	float* pHeightMap = new float[image->height * image->width];
	for(int i = 0; i < image->height * image->width; i++){
		pHeightMap[i] = ((float)((unsigned char)image->pixels[3*i])) * HEIGHT_SCALE;
		/*if(pHeightMap[i] < 0)
			int S = 4;*/
	}
	// create a new TerrainObject
	TerrainObject* terrain = new TerrainObject(image->width, image->height, STRIDE);
	// set texture
	GLuint tex = TextureManager::getInstance()->getTextureId("resource/Textures/terrain.bmp");
	terrain->setTexId(tex);
	// set height map
	terrain->setHeightMap(pHeightMap);
	// set materail
	//terrain->setMaterail(SOIL_TERRAIN);

	// get ObjectManager
	ObjectManager* om = ObjectManager::getInstance();
	// add terrain into ObjectManager
	om->addVBOObject(terrain);
	return true;
}
