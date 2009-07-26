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

	// this load function is only for loading of character kind model
	// path1 record skeleton skin and one animation information
	// path2 and path3 each records one corresponding animation to the skeleton
	// in path1
	virtual bool loadModel(const char* path1, const char* path2, const char* path3) = 0;
};
