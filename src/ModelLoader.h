#pragma once

class ModelLoader
{
public:
    /*
	 * para: path: load a model with the given path
	 *       kind: specifies the role of the model in the entire world(e.g terrain,character etc.)
	 * function: load the model with the given path and set it in the mesh manager
	 * return: true when model is successfully loaded, vice versa
	 */
	virtual bool loadModel(int kind, char* path) = 0;
};
