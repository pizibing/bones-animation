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
#include "FCDocument\FCDCamera.h"
#include "FCDocument\FCDGeometryPolygonsTools.h"
#include "FCDocument\FCDGeometryPolygonsInput.h"
#include "FUtils\FUObject.h" 
#include "FCDocument\FCDGeometryInstance.h"
#include "FCDocument\FCDMaterial.h" 
#include "FCDocument\FCDMaterialInstance.h"
#include "FCDocument\FCDEffect.h"
#include "FCDocument\FCDEffectProfile.h"
#include "FCDocument\FCDEffectStandard.h"
#include "FCDocument\FCDLight.h" 
#include "FCDocument\FCDImage.h"
#include "FCDocument\FCDTexture.h"
#include "FCDocument\FCDAnimation.h"
#include "FCDocument\FCDAnimated.h"
#include "FCDocument\FCDAnimationKey.h"
#include "FCDocument\FCDAnimationCurve.h"
#include "FCDocument\FCDAnimationChannel.h"
#include "FCDocument\FCDTransform.h"
#include "FCDocument\FCDControllerInstance.h"
#include "FCDocument\FCDSkinController.h"
#include "FCDocument\FCDController.h"
#include "FMath\FMInterpolation.h"
#include "FMath\FMMatrix44.h"

class Vector3D;
class ModelLoader;
class ObjectManager;
class LineObject;
class Matrix;

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
	void storeVertices(int kind);

	void storeLight();

	//store all the textures that contains in the document
	void storeTexture();

	//store all the materials that contains in the document
	void storeMaterials();

	void storeCamera();

	void storeAnimation();

	//build the scene include the material of the polygons, the texture of the polygons and the matrix of the bone.
	void buildScene(FCDSceneNode* ptr_root, int kind);

	void buildSceneInstance(FCDSceneNode* ptr_root, int kind);

	void buildSkin(FCDSkinController* skin);

	void buildSceneMatrix(FCDSceneNode* ptr_root);

	//set the material of the mesh
	void setMeshFCMaterial(FCDGeometryInstance* geometry_instance, FCDGeometryMesh* mesh, int meshIndex, int kind);

	//set the target material to the polygon
	void setFCMaterial(int j, int target, int polygonIndex, int kind);

	void drawLine(FCDSceneNode* node_origin, int kind);

	//search the texture in the texture lib by texture id
	FCDImage *SearchTextureByName(fm::string textureid);

	void BuildCharacter();

	int getBoneNumber();

	void initBoneScene(FCDSceneNode* node_origin);

	int getBoneIndexByName(std::string name);

	int * getSkinVertexIndex(int begin, int end);

	std::vector<SimpleLine*> simpleLines;

	// convert FMMatrix44 to Matrix
	inline Matrix convertToMatrix(const FMMatrix44 & fmmatrix){
		Matrix m;
		for(int i=0;i<16;i++){
			m[i]=fmmatrix.m[i/4][i%4];
		}
		return m;
	} 

private:
	// pointer to dae file that will be opened using fcollada
	FCDocument* m_document;

	//numbeer of textures
	int m_num_textures;
	std::vector<FCDImage*> m_ptrs_textures;
	std::vector<GLfloat*> m_total_texcoords;
	std::vector<int> m_size_texcoords;

	std::vector<FCDGeometryMesh*> m_ptrs_geometries;

	// materials
	int m_num_materials;
	std::vector<FCDMaterial*> m_ptrs_materials;

	int m_num_lights;
	std::vector<FCDLight*> m_ptrs_Ambientlights;
	std::vector<FCDLight*> m_ptrs_Spotlights;
	std::vector<FCDLight*> m_ptrs_Directionallights;
	std::vector<FCDLight*> m_ptrs_Pointlights;

	int m_num_cameras; // cameras in library, not SG
	// structures to store cameras
	std::vector<FCDCamera*> m_ptrs_Perspectivecameras;
	std::vector<FCDCamera*> m_ptrs_Orthographiccameras;

	int m_num_animations;
	std::string* animationsBoneName; 
	std::vector<FCDAnimation*> m_ptrs_animation;

	//character value
	int boneNumber;
	std::string* boneName; 
	Matrix* boneMatrix;
	std::string* boneParentName; 
	std::string** boneChildName; 
	int* boneChildNum;

	std::string filename;
	int* animationsBoneFrameNum;
	Matrix** animationsBoneFrameMatrix;
	int** animationsBoneFrameTime;

	int skinVertexNum;
	float* skinVertexPosX;
	float* skinVertexPosY;
	float* skinVertexPosZ;
	float* skinNormalPosX;
	float* skinNormalPosY;
	float* skinNormalPosZ;
	int* skinPolygonIndex;
	int* skinPolygonVertexIndex;
	int* skinVertexBoneCount;
	std::string** skinVertexBoneName;
	float** skinVertexBonePower;

	int charactervboNum;
	int character_vbo_size;
	float ** character_vbo_vertices;
	float ** character_vbo_normals;
	float ** character_vbo_texcoords;
	float** character_vbo_am;
	float** character_vbo_di;
	float** character_vbo_sp;
	float** character_vbo_em;
	float* character_vbo_sh;
	GLuint* character_vbo_texid;

	//objectmanage
	ObjectManager* objectManager;
};
