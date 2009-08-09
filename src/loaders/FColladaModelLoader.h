#pragma once

#define NO_LIBXML
#define ROOT_BONE_NAME "rootbone"

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
class ObjectManager;
class LineObject;
class Matrix;

static const int sigle_frame_time = 30;

//it contains the fuction to load from the fcollada data file
class FColladaModelLoader
{
public:
	FColladaModelLoader(void);
	~FColladaModelLoader(void);

	//load the model from a path of a fcollada file
	//return ture if load success
	//set the document value while load the model
	//it also contain three methed storeVertices, storeTexture, storeMaterials. which
	//store the vertices, texture, materials to my own structure
	bool loadModel(int kind, int num, const char** szPathName);

	//store the vertices, normals, texturecoords and create staticobjects to display
	void storeVertices(int kind, FCDocument* m_document);

	//store all the textures that contains in the document
	void storeTexture(FCDocument* m_document);

	//store all the materials that contains in the document
	void storeMaterials(FCDocument* m_document);

	//build the scene include the material of the polygons, the texture of the polygons and the matrix of the bone.
	//first pass in a set of root node, then look through all the scene node by find the scene node's child node
	//for every set of root node, 
	//load the instance from the first root node, then store the information of the instance to my struct
	//load the right matrix information and the animation information from the set of the node, then 
	//store a set of animation matrix for the set of node, a position matrix for the whole scene node
	//load the bone information from the scene node. 
	void buildScene(FCDSceneNode** node_origin, int kind);

	//set the skin instance information and geo instance information
	//contains the skin information, weight information, the vbo information
	//it set the value: skinPolygonIndex, skinVertexNum, skinVertexPosX,
	//skinVertexPosY, skinVertexPosZ, skinNormalPosX, skinNormalPosY, skinNormalPosZ,
	//skinVertexBoneCount, skinPolygonVertexIndex, skinVertexBoneName, skinVertexBonePower,
	//it init the value charactervboNum, character_vbo_vertices, character_vbo_normals, character_vbo_texcoords,
	//character_vbo_am, character_vbo_di, character_vbo_sp, character_vbo_em, character_vbo_sh, character_vbo_texid,
	void buildSceneInstance(FCDSceneNode* ptr_root, int kind);

	//build the bone information, the inverse information
	//it set the value boneNumber boneName boneMatrix boneInverseMatrix boneParentName 
	//boneChildNum boneChildName
	//it init the value animationsBoneFrameNum animationsBoneFrameMatrix animationsBoneFrameTime.
	void buildSkin(FCDSkinController* skin);

	//build the matrix of the node, include the matrix for the node and the animation information
	//it store the value animationsBoneFrameNum animationsBoneFrameMatrix animationsBoneFrameTime
	//rootAnimationsBoneFrameNum rootAnimationsBoneFrameMatrix rootAnimationsBoneFrameTime;
	void buildSceneMatrix(FCDSceneNode* ptr_root, int m_documentIndex);

	//set the material of the mesh
	void setMeshFCMaterial(FCDGeometryInstance* geometry_instance, FCDGeometryMesh* mesh, int meshIndex, int kind);

	//set the target material and texture to the polygon
	void setFCMaterial(int j, int target, int polygonIndex, int kind);

	//search the texture in the texture lib by texture id
	FCDImage *SearchTextureByName(fm::string textureid);

	//build the character object
	// first build the skeleton, then build the animation,
	//then build the skin, then build the vbo objects
	//at last, put the object to my object manager
	void BuildCharacter();

	//store the information about root bone and joint bone.
	// the information include the parent name, children number and children name
	//the value contains boneParentName, boneChildName and boneChildNum.
	void initBoneScene(FCDSceneNode* node_origin);

	//search the index of name in the boneName value , return -1, if can't find the name in it
	int getBoneIndexByName(std::string name);

	//set the index of the vertex in the vertex lib in my struct
	int * getSkinVertexIndex(int begin, int end);

	//delete the temp value
	void deleteTempValue();

	// convert FMMatrix44 to Matrix
	inline Matrix convertToMatrix(const FMMatrix44 & fmmatrix){
		Matrix m;
		for(int i=0;i<16;i++){
			m[i]=fmmatrix.m[i/4][i%4];
		}
		return m;
	} 

private:
	int szFlieNum;
	// pointer to dae file that will be opened using fcollada
	FCDocument** m_document;

	//number of textures
	int m_num_textures;
	std::vector<FCDImage*> m_ptrs_textures;
	std::vector<GLfloat*> m_total_texcoords;
	std::vector<int> m_size_texcoords;

	std::vector<FCDGeometryMesh*> m_ptrs_geometries;

	// materials
	int m_num_materials;
	std::vector<FCDMaterial*> m_ptrs_materials;

	std::string* animationsBoneName; 

	//bone value
	int boneNumber;
	std::string* boneName; 
	Matrix* boneMatrix;
	Matrix* boneInverseMatrix;
	std::string* boneParentName; 
	std::string** boneChildName; 
	int* boneChildNum;
	
	//root bone value
	std::string rootBoneName;
	bool* isRootBoneName;
	Matrix rootBoneMatrix;
	bool isBootBoneSceneNode;
	int rootBoneChildNum;
	std::string* rootBoneChildName;

	//root bone animation
	int* rootAnimationsBoneFrameNum;
	Matrix** rootAnimationsBoneFrameMatrix;
	int** rootAnimationsBoneFrameTime;

	//file name
	std::string* szfilename;

	//bone animation
	int** animationsBoneFrameNum;
	Matrix*** animationsBoneFrameMatrix;
	int*** animationsBoneFrameTime;

	//skin 
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

	//skin vertex power
	float** skinVertexBonePower;

	//character vbo
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

	//object manage
	ObjectManager* objectManager;
};
