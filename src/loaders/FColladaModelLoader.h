#pragma once

#define NO_LIBXML

// disable deprecated warning 
#pragma warning( disable : 4996 )

#include "FCollada.h"

// FCollada
#include "FCDocument\FCDocument.h"
#include "FCDocument\FCDLibrary.h"
#include "FCDocument\FCDGeometryMesh.h"
#include "FCDocument\FCDGeometry.h"
#include "FCDocument\FCDGeometryPolygons.h"
#include "FCDocument\FCDGeometrySource.h"
#include "FCDocument\FCDSceneNode.h" 
#include "FCDocument\FCDGeometryPolygonsTools.h"
#include "FCDocument\FCDGeometryPolygonsInput.h"
#include "FUtils\FUObject.h" 

#include "ModelLoader.h"
#include "../managers/MeshManager.h"
#include "../data/MyPolygon.h"

class FColladaModelLoader :
	public ModelLoader
{
public:
	FColladaModelLoader(void);
	~FColladaModelLoader(void);

	bool loadModel(int kind, char* path);

private:
	bool m_is_file_opened; 

	// pointer to dae file that will be opened using fcollada
	FCDocument* m_document;

	int m_num_geometries;

	MeshManager* meshManager;
};
