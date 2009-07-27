#pragma once

class FColladaModelLoader;
class TerrainModelLoader;

// define load kind
const int CHARACTER_LOADER_KIND = 0;
const int TERRAIN_LOADER_KIND = 1;
const int STATIC_LOADER_KIND = 2;

// CommandModelLoader is a ModelLoader that take order and 
// arranges those order to other ModelLoaders
// it is just a commander, actually it does nothing concrete
class CommandModelLoader: public ModelLoader
{
public:
	
	// get the single instance of CommandModelLoader
	static CommandModelLoader* getInstance();

	/*
	* para: path: load a model with the given path
	*       kind: specifies the role of the model in the entire world(e.g terrain,character etc.)
	* function: load the model with the given path and set it in the ObjectManager
	* return: true when model is successfully loaded, vice versa
	*/
	bool loadModel(int kind, const char* path);
	bool loadMulModel(int kind, int num, const char** szPathName);

	// this load function is only for loading of character kind model
	// path1 record skeleton skin and one animation information
	// path2 and path3 each records one corresponding animation to the skeleton
	// in path1
	bool loadModel(const char* path1, const char* path2, const char* path3);

private:
	// constructor
	CommandModelLoader(void);
	// destructor
	~CommandModelLoader(void);

	// three kinds of loaders
	FColladaModelLoader* floader;
	TerrainModelLoader* tloader;

	// single instance of this class
	static CommandModelLoader* singleton;
};
