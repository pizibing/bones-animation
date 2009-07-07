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
#include "FCDocument\FCDGeometryInstance.h"
#include "FCDocument\FCDMaterial.h" 
#include "FCDocument\FCDMaterialInstance.h"
#include "FCDocument\FCDEffect.h"
#include "FCDocument\FCDEffectProfile.h"
#include "FCDocument\FCDEffectStandard.h"
#include "FCDocument\FCDLibrary.h"
#include "FCDocument\FCDImage.h"

class ModelLoader;
class ObjectManager;

//it contains the fuction to load from the fcollada data file
class FColladaModelLoader :
	public ModelLoader
{
public:
	FColladaModelLoader(void);
	~FColladaModelLoader(void);

	//load the model from a path of a fcollada file
	//return ture if load success
	//set the document value while load the model
	//it also contain three methed storeVertices, storeTexture, storeMaterials. which
	//store the vertices, texture, materials to my own structure 
	bool loadModel(int kind,const char* path);

	//store the vertices, normals, texturecoords and create staticobjects to display
	void storeVertices(FCDocument* m_document);

	//store all the textures that contains in the document
	void storeTexture(FCDocument* m_document);

	//store all the materials that contains in the document
	void storeMaterials(FCDocument* m_document);

	void buildScene(FCDSceneNode* ptr_root);

	int getFCMaterial(FCDGeometryInstance* geometry_instance, FCDGeometryMesh* mesh);

	void setFCMaterial(int j, int target);

private:
	// pointer to dae file that will be opened using fcollada
	FCDocument* m_document;

	//numbeer of textures
	int m_num_textures;
	std::vector<FCDImage*> m_ptrs_textures;

	std::vector<FCDGeometryMesh*> m_ptrs_geometries;

	// materials
	int m_num_materials;
	std::vector<FCDMaterial*> m_ptrs_materials;

	//objectmanage
	ObjectManager* objectManager;
};
