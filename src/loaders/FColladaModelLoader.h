#pragma once

#define NO_LIBXML

// disable deprecated warning 
#pragma warning( disable : 4996 )

#include "FCollada.h"
#include "FCDocument\FCDocument.h"

#include "ModelLoader.h"
#include "../managers/MeshManager.h"

class FColladaModelLoader :
	public ModelLoader
{
public:
	FColladaModelLoader(void);
	~FColladaModelLoader(void);

	// pointer to dae file taht will be opened using fcollada
	FCDocument* m_document;

	bool loadModel(int kind, char* path);

private:
	bool m_is_file_opened; 
	MeshManager* meshManager;
};
