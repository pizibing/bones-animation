#pragma once

// TerrainModelLoader load terrain object
class TerrainModelLoader
{
public:
	// constructor
	TerrainModelLoader(void);
	// destructor
	~TerrainModelLoader(void);

	/*
	* para: path: load a model with the given path
	*       kind: specifies the role of the model in the entire world(e.g terrain,character etc.)
	* function: load the model with the given path and set it in the ObjectManager
	* return: true when model is successfully loaded, vice versa
	*/
	bool loadModel(int kind, const char* path);
};
