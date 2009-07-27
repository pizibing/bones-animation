#pragma once

// ModelLoader is a interface that should be implemented by
// all ModelLoaders. 
// it defines the load function of ModelLoader
class ModelLoader
{
public:
    /*
	 * para: path: load a model with the given path
	 *       kind: specifies the role of the model in the entire world(e.g terrain,character etc.)
	 * function: load the model with the given path and set it in the ObjectManager
	 * return: true when model is successfully loaded, vice versa
	 */
	virtual bool loadModel(int kind, const char* path) = 0;

	// load multiple models at one time
	// kind is the role of the model in the entire world. e.g.TERRAIN_LOADER_KIND,
	// CHARACTER_LOADER_KIND, STATIC_LOADER_KIND
	// num is the number of models to load
	// szPathName is the path names of those model to load
	// if kind is CHARACTER_LOADER_KIND, then the first model will be treated
	// as a normal character while the others will be treated as animations of
	// the first character.
	virtual bool loadMulModel(int kind, int num, const char** szPathName) = 0;
};
