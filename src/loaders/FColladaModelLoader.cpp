#include <olectl.h>	
#include <assert.h>
#include <string>
#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "../data/VBOMesh.h"
#include "../data/SimpleLine.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../data/MoveSelfObject.h"
#include "../data/static/StaticObject.h"
#include "../data/character/ChVertex.h"
#include "../data/character/ChSkin.h"
#include "../data/character/ChBone.h"
#include "../data/character/ChSkeleton.h"
#include "../data/character/ChKeyFrame.h"
#include "../data/character/ChTrack.h"
#include "../data/character/ChAnimation.h"
#include "../data/character/ChAnimationManager.h"
#include "../data/character/CharacterObject.h"
#include "../managers/ObjectManager.h"
#include "../managers/TextureManager.h"
#include "FCVector4D.h"
#include "FColladaModelLoader.h"

//constructor
FColladaModelLoader::FColladaModelLoader(void){

	//initialize the Fcollada namespace
	FCollada::Initialize(); 

	boneNumber = 0;
	skinVertexNum = 0;
	character_vbo_size = 0;
	charactervboNum = 0;
	m_num_materials = 0;
	m_num_textures = 0;
	rootBoneName = ROOT_BONE_NAME;
	isBootBoneSceneNode = false;
}

//destructor
FColladaModelLoader::~FColladaModelLoader(void){	
	//document
	for(int j=0; j < szFlieNum; j++)
	{
		delete[] m_document[j];
		m_document[j] = NULL;
	}
	delete[] m_document;
	m_document = NULL;

	//bone value
	delete[] boneName;
	delete[] boneMatrix;
	delete[] boneInverseMatrix;
	delete[] boneParentName;
	delete[] boneChildNum;

	//bone child name
	for(int i=0; i < boneNumber; i++)
	{
		delete[] boneChildName[i];
		boneChildName[i] = NULL;
	}
	delete boneChildName;
	boneChildName = NULL;

	//root bone value
	delete[] isRootBoneName;
	delete[] rootBoneChildName;

	//root bone animation
	for(int j=0; j < szFlieNum; j++)
	{
		if(rootAnimationsBoneFrameNum[j] != 0)
		{
			delete[] rootAnimationsBoneFrameMatrix[j];
			rootAnimationsBoneFrameMatrix[j] = NULL;
			delete[] rootAnimationsBoneFrameTime[j];
			rootAnimationsBoneFrameTime[j] = NULL;
		}
	}
	delete[] rootAnimationsBoneFrameNum;
	rootAnimationsBoneFrameNum = NULL;

	//file name
	delete[] szfilename;

	//bone animation
	for(int j=0; j < szFlieNum; j++)
	{
		for(int i=0; i < boneNumber; i++)
		{
			if(animationsBoneFrameNum[j][i] != 0)
			{
				delete[] animationsBoneFrameMatrix[j][i];
				animationsBoneFrameMatrix[j][i] = NULL;
				delete[] animationsBoneFrameTime[j][i];
				animationsBoneFrameTime[j][i] = NULL;
			}
		}
		delete[] animationsBoneFrameMatrix[j]; 
		animationsBoneFrameMatrix[j] = NULL;

		delete[] animationsBoneFrameTime[j];
		animationsBoneFrameTime[j] = NULL;

		delete[] animationsBoneFrameNum[j];
		animationsBoneFrameNum[j] = NULL;
	}
	delete[] animationsBoneFrameMatrix;
	animationsBoneFrameMatrix = NULL;
	delete[] animationsBoneFrameTime;
	animationsBoneFrameTime = NULL;
	delete[] animationsBoneFrameNum;
	animationsBoneFrameNum = NULL;

	//character skin
	delete[] skinVertexPosX;
	delete[] skinVertexPosY;
	delete[] skinVertexPosZ;
	delete[] skinNormalPosX;
	delete[] skinNormalPosY;
	delete[] skinNormalPosZ;
	delete[] skinPolygonIndex;
	delete[] skinPolygonVertexIndex;
	delete[] skinVertexBoneCount;

	for(int i=0; i < skinVertexNum; i++)
	{
		delete[] skinVertexBoneName[i];
		skinVertexBoneName[i] = NULL;
	}
	delete skinVertexBoneName; 
	skinVertexBoneName = NULL;

	for(int i=0; i < skinVertexNum; i++)
	{
		delete[] skinVertexBonePower[i];
		skinVertexBonePower[i] = NULL;
	}
	delete skinVertexBonePower; 
	skinVertexBonePower = NULL;

	//character vbo
	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_vertices[i];
		character_vbo_vertices[i] = NULL;
	}
	delete character_vbo_vertices; 
	character_vbo_vertices = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_normals[i];
		character_vbo_normals[i] = NULL;
	}
	delete[] character_vbo_normals; 
	character_vbo_normals = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_texcoords[i];
		character_vbo_texcoords[i] = NULL;
	}
	delete[] character_vbo_texcoords; 
	character_vbo_texcoords = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_am[i];
		character_vbo_am[i] = NULL;
	}
	delete[] character_vbo_am; 
	character_vbo_am = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_di[i];
		character_vbo_di[i] = NULL;
	}
	delete[] character_vbo_di; 
	character_vbo_di = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_sp[i];
		character_vbo_sp[i] = NULL;
	}
	delete character_vbo_sp; 
	character_vbo_sp = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
		delete[] character_vbo_em[i];
		character_vbo_em[i] = NULL;
	}
	delete[] character_vbo_em; 
	character_vbo_em = NULL;

	delete[] character_vbo_sh;
	delete[] character_vbo_texid;
}

//load the model from a path of a fcollada file
//return ture if load success
//set the document value while load the model
//it also contain three methed storeVertices, storeTexture, storeMaterials. which
//store the vertices, texture, materials to my own structure 
//init the value m_document, szfilename, animationsBoneFrameNum,animationsBoneFrameMatrix
//animationsBoneFrameTime, rootAnimationsBoneFrameNum, rootAnimationsBoneFrameMatrix,
//rootAnimationsBoneFrameTime, isRootBoneName
bool FColladaModelLoader::loadModel(int kind, int num, const char** szPathName){
	//set the File num that we used
	szFlieNum = num;

	//initialize the values
	m_document= new FCDocument*[szFlieNum];
	szfilename = new std::string[szFlieNum];
	animationsBoneFrameNum = new int*[szFlieNum];
	animationsBoneFrameMatrix = new Matrix**[szFlieNum];
	animationsBoneFrameTime = new int**[szFlieNum];
	rootAnimationsBoneFrameNum = new int[szFlieNum];
	rootAnimationsBoneFrameMatrix = new Matrix*[szFlieNum];
	rootAnimationsBoneFrameTime = new int*[szFlieNum];
	isRootBoneName = new bool[szFlieNum];

	//set the isRootBoneName flag for setting the root bone
	for(int i = 0; i < szFlieNum; i++)
	{
		isRootBoneName[i] = true;
	}

	//it is a class value, which administrate the object
	objectManager = ObjectManager::getInstance();

	//look through the files, get the right file name and set the document
	for(int i = 0; i < szFlieNum; i++)
	{
		//get the origin name
		std::string tempFilename = szPathName[i];
		//get the file name to tell from animations and store the value to my struct
		int tempFileNameIndex = tempFilename.find_last_of("/");
		szfilename[i] = tempFilename.substr(tempFileNameIndex+1, tempFilename.length()-5-tempFileNameIndex);
		// new dae file
		m_document[i] = FCollada::NewTopDocument();

		//path in unicode
		OLECHAR		wszPath[MAX_PATH+1];

		//temp path
		char		szPath[MAX_PATH+1];	

		//if the path contains the http:// , then copy the path to the szPath 
		if (strstr(szPathName[i], "http://"))									
		{
			strcpy(szPath, szPathName[i]);										
		}
		// else load the model from the file
		else																
		{
			// get the current path
			GetCurrentDirectoryA(MAX_PATH, szPath);	

			//add the '\'
			strcat(szPath, "\\");

			//add the relative path
			strcat(szPath, szPathName[i]);										
		}

		//convert ascii to unicode
		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);

		//convert the type of path to fstring
		fstring fpath=FUStringConversion::ToFString(wszPath);

		//load the document from the file. return true, if success
		bool ret=FCollada::LoadDocumentFromFile(m_document[i], fpath);	

		assert(ret);
	}
	//store the vertices, normals, texturecoords and create staticobjects to display
	storeVertices(kind, m_document[0]);

	//store all the textures that contains in the document
	storeTexture(m_document[0]);

	//store all the materials that contains in the document
	storeMaterials(m_document[0]);

	//build the scene
	//set the root of the scene 
	FCDSceneNode** ptr_root = new FCDSceneNode*[szFlieNum];
	for(int j = 0; j < szFlieNum; j++)
	{
		ptr_root[j]=m_document[j]->GetVisualSceneRoot();
	}
	
	//build the scene through the root
	if(ptr_root[0]!=NULL)
	{
		buildScene(ptr_root, 0);
	}

	//build the character object
	if(kind == 0)
	{
		BuildCharacter();
	}

	//delete the temp value
	deleteTempValue();

	return true;
}


//delete the temp value
void FColladaModelLoader::deleteTempValue()
{
	//texture
	m_ptrs_textures.clear();
	m_total_texcoords.clear();
	m_size_texcoords.clear();

	//geometries
	m_ptrs_geometries.clear();

	// materials
	m_ptrs_materials.clear();

	//document
	for(int j=0; j < szFlieNum; j++)
	{
		delete[] m_document[j];
		m_document[j] = NULL;
	}
	delete[] m_document;
	m_document = NULL;

	//bone value
	delete[] boneName;
	delete[] boneMatrix;
	delete[] boneInverseMatrix;
	delete[] boneParentName;
	delete[] boneChildNum;

	//bone child name
	for(int i=0; i < boneNumber; i++)
	{
	delete[] boneChildName[i];
	boneChildName[i] = NULL;
	}
	delete boneChildName;
	boneChildName = NULL;
	
	//root bone value
	delete[] isRootBoneName;
	delete[] rootBoneChildName;

	//root bone animation
	for(int j=0; j < szFlieNum; j++)
	{
		if(rootAnimationsBoneFrameNum[j] != 0)
		{
			delete[] rootAnimationsBoneFrameMatrix[j];
			rootAnimationsBoneFrameMatrix[j] = NULL;
			delete[] rootAnimationsBoneFrameTime[j];
			rootAnimationsBoneFrameTime[j] = NULL;
		}
	}
	delete[] rootAnimationsBoneFrameNum;
	rootAnimationsBoneFrameNum = NULL;

	//file name
	delete[] szfilename;

	//bone animation
	for(int j=0; j < szFlieNum; j++)
	{
		for(int i=0; i < boneNumber; i++)
		{
			if(animationsBoneFrameNum[j][i] != 0)
			{
				delete[] animationsBoneFrameMatrix[j][i];
				animationsBoneFrameMatrix[j][i] = NULL;
				delete[] animationsBoneFrameTime[j][i];
				animationsBoneFrameTime[j][i] = NULL;
			}
		}
		delete[] animationsBoneFrameMatrix[j]; 
		animationsBoneFrameMatrix[j] = NULL;

		delete[] animationsBoneFrameTime[j];
		animationsBoneFrameTime[j] = NULL;

		delete[] animationsBoneFrameNum[j];
		animationsBoneFrameNum[j] = NULL;
	}
	delete[] animationsBoneFrameMatrix;
	animationsBoneFrameMatrix = NULL;
	delete[] animationsBoneFrameTime;
	animationsBoneFrameTime = NULL;
	delete[] animationsBoneFrameNum;
	animationsBoneFrameNum = NULL;
	
	//character skin
	delete[] skinVertexPosX;
	delete[] skinVertexPosY;
	delete[] skinVertexPosZ;
	delete[] skinNormalPosX;
	delete[] skinNormalPosY;
	delete[] skinNormalPosZ;
	delete[] skinPolygonIndex;
	delete[] skinPolygonVertexIndex;
	delete[] skinVertexBoneCount;

	for(int i=0; i < skinVertexNum; i++)
	{
	delete[] skinVertexBoneName[i];
	skinVertexBoneName[i] = NULL;
	}
	delete skinVertexBoneName; 
	skinVertexBoneName = NULL;

	for(int i=0; i < skinVertexNum; i++)
	{
	delete[] skinVertexBonePower[i];
	skinVertexBonePower[i] = NULL;
	}
	delete skinVertexBonePower; 
	skinVertexBonePower = NULL;

	//character vbo
	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_vertices[i];
	character_vbo_vertices[i] = NULL;
	}
	delete character_vbo_vertices; 
	character_vbo_vertices = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_normals[i];
	character_vbo_normals[i] = NULL;
	}
	delete[] character_vbo_normals; 
	character_vbo_normals = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_texcoords[i];
	character_vbo_texcoords[i] = NULL;
	}
	delete[] character_vbo_texcoords; 
	character_vbo_texcoords = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_am[i];
	character_vbo_am[i] = NULL;
	}
	delete[] character_vbo_am; 
	character_vbo_am = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_di[i];
	character_vbo_di[i] = NULL;
	}
	delete[] character_vbo_di; 
	character_vbo_di = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_sp[i];
	character_vbo_sp[i] = NULL;
	}
	delete character_vbo_sp; 
	character_vbo_sp = NULL;

	for(int i=0; i < charactervboNum; i++)
	{
	delete[] character_vbo_em[i];
	character_vbo_em[i] = NULL;
	}
	delete[] character_vbo_em; 
	character_vbo_em = NULL;

	delete[] character_vbo_sh;
	delete[] character_vbo_texid;
}

//build the character object
// first build the skeleton, then build the animation,
//then build the skin, then build the vbo objects
//at last, put the object to my object manager
void FColladaModelLoader::BuildCharacter()
{
	// create a new CharacterObject
	CharacterObject* character = new CharacterObject();

	/* step1: initialize skeleton */
	// create a new ChSkeleton
	ChSkeleton* skeleton = character->getSkeleton();
	// get bone number of the skeleton from Fcollada
	int bone_num = 0;

	bone_num = boneNumber;
	//	printf("boneNumber\n");
	//	printf("%i\n", boneNumber);
	//	printf("\n");
	// set bone number to skeleton
	skeleton->init(bone_num+1);

	ChBone* rootBone = skeleton->getBone(ROOT_BONE_NAME);
	Matrix matrix = rootBoneMatrix;
	rootBone->setTransformMatrix(matrix);
	rootBone->setBindPoseInverse(matrix.getInverseMatrix());
	int rootChild_num = rootBoneChildNum;
	//		printf("boneChildNum\n");
	//		printf("%i\n", child_num);
	// set child number
	rootBone->init(rootChild_num);
	// for each child
	for(int i = 0; i < rootChild_num; i++){
		// get child's name from Fcollada
		std::string childName = rootBoneChildName[i];
		//			printf("boneChildName\n");
		//			printf("%s\n", childName.c_str());
		// get child's pointer
		ChBone* child = skeleton->getBone(childName);
		// set child
		rootBone->addChildBone(child);
	}
	//		printf("\n");
	// for each bone
	for(int i = 0; i < bone_num; i++){
		/* create a new ChBone, the name of the bone is necessary */
		// get bone name from Fcollada
		std::string name = boneName[i];
		//		printf("boneName\n");
		//		printf("%s\n", name.c_str());
		ChBone* bone = skeleton->getBone(name);
		/* initialize the bone */
		// get relative transform matrix of the bone from Fcollada
		Matrix matrix = boneMatrix[i];

		//		printf("boneMatrix\n");
		//		printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		//		matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12], 
		//		matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13], 
		//		matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14], 
		//		matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15]);
		// set bone's relative transform matrix 
		bone->setTransformMatrix(matrix);
		bone->setBindPoseInverse(boneInverseMatrix[i]);
		// get parent's name from Fcollada
		std::string parentName = boneParentName[i];
		//		printf("boneParentName\n");
		//		printf("%s\n", parentName.c_str());
		if(parentName.length() > 0)
		{
			// get parent's pointer
			ChBone* parent = skeleton->getBone(parentName);
			// set parent
			bone->SetParentBone(parent);
		}
		else{
			ChBone* parent = skeleton->getBone(ROOT_BONE_NAME);
			bone->SetParentBone(parent);
		}
		// get child number from Fcollada
		int child_num = boneChildNum[i];
		//		printf("boneChildNum\n");
		//		printf("%i\n", child_num);
		// set child number
		bone->init(child_num);
		// for each child
		for(int j = 0; j < child_num; j++){
			// get child's name from Fcollada
			std::string childName = boneChildName[i][j];
			//			printf("boneChildName\n");
			//			printf("%s\n", childName.c_str());
			// get child's pointer
			ChBone* child = skeleton->getBone(childName);
			// set child
			bone->addChildBone(child);
		}
		//		printf("\n");
	}
	// set root bone
	skeleton->setRootBone(rootBone);
	// calculate tranform in world space
	skeleton->calculateAbsoluteTransform();
	//	printf("\n");

	/* step2: initialize animations */
	// create a new animation manager
	ChAnimationManager* animations = character->getAnimations();
	// load animations from this file

	for(int p=0; p<szFlieNum; p++)
	{
		// create a new animation, the name of the animation is necessary
		// get name of the animation from Fcollada
		// you can take the file name as animation name simply
		std::string animationName = szfilename[p];
		printf("animationName\n");
		printf("%s\n", animationName.c_str());
		ChAnimation* animation = animations->getAnimation(animationName);
		/* each bone of the skeleton has one corresponding ChTrack in one
		animation which records the movement of the bone */
		//set rootbone animation
		// get bone's name
		std::string animationboneName = ROOT_BONE_NAME;
		//		printf("boneName\n");
		//		printf("%s\n", animationboneName.c_str());
		// get bone's relative ChTrack
		ChTrack* track = animation->getTrack(animationboneName);
		/* initialize the track */
		// get key frame number of this track from Fcollada
		// usually is the animation total key frame number
		int rootFrame_num = rootAnimationsBoneFrameNum[p];
		//		printf("rootFrame_num\n");
		//		printf("%i\n", rootFrame_num);
		if(rootFrame_num != 0)
		{
			// set key frame number
			track->init(rootFrame_num);
			// for each key frame
			for(int i=0;i<rootFrame_num;i++){
				// get relative change matrix of the bone in this key frame
				// from Fcollada
				Matrix matrix = rootAnimationsBoneFrameMatrix[p][i];
				//			printf("rootBoneMatrix\n");
				//			printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
				//			matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12], 
				//			matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13], 
				//			matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14], 
				//			matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15]);
				// get this key frame's frame number in the whole animation
				// from Fcollada
				int frame_time = rootAnimationsBoneFrameTime[p][i];
				//			printf("rootFrame_time\n");
				//			printf("%i\n", frame_time);
				// add the key frame into the track
				track->addKeyFrame(matrix,frame_time);
			}
		}
		// for each bone (how to go through all the bones is your choice)
		for(int i=0;i<bone_num;i++){
			// get bone's name
			std::string animationboneName = boneName[i];
			//		printf("boneName\n");
			//		printf("%s\n", animationboneName.c_str());
			// get bone's relative ChTrack
			ChTrack* track = animation->getTrack(animationboneName);
			/* initialize the track */
			// get key frame number of this track from Fcollada
			// usually is the animation total key frame number
			int frame_num = animationsBoneFrameNum[p][i];
			//		printf("frame_num\n");
			//		printf("%i\n", frame_num);
			// set key frame number
			track->init(frame_num);
			// for each key frame
			for(int j=0;j<frame_num;j++){
				// get relative change matrix of the bone in this key frame
				// from Fcollada
				Matrix matrix = animationsBoneFrameMatrix[p][i][j];
				//			printf("boneMatrix\n");
				//			printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
				//			matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12], 
				//			matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13], 
				//			matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14], 
				//			matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15]);
				// get this key frame's frame number in the whole animation
				// from Fcollada
				int frame_time = animationsBoneFrameTime[p][i][j];
				//			printf("frame_time\n");
				//			printf("%i\n", frame_time);
				// add the key frame into the track
				track->addKeyFrame(matrix,frame_time);
			}
			//		printf("\n");
		}
	}
	/* step3: initialize skin */
	// create new ChSkin
	ChSkin* skin = character->getSkin();
	// get vertex number of the skin from Fcollada
	int vertexNum = skinVertexNum;
	//	printf("vertexNum\n");
	//	printf("%i\n", vertexNum);
	//	printf("\n");
	// set vertex number(no duplicated vertex is allowed)
	skin->initVertices(vertexNum);
	// initialize each vertex in the skin
	for(int i = 0; i < vertexNum; i++){
		// create a vertex
		ChVertex* vertex = skin->initVertice(i);
		// get position of the vertex to be added from Fcollada
		float px = skinVertexPosX[i];
		float py = skinVertexPosY[i];
		float pz = skinVertexPosZ[i];
		//		printf("%i vertex position\n", i);
		//		printf("(%f, %f, %f)\n", px, py, pz);
		// create a new vertex
		vertex->setDefaultPosition(px,py,pz);
		// get normal of the vertex from Fcollada
		float nx = skinNormalPosX[i];
		float ny = skinNormalPosY[i];
		float nz = skinNormalPosZ[i];
		// set normal
		vertex->setDefaultNormal(nx,ny,nz);
		// get number of bones this vertex relates to from Fcollada
		int relatedBoneNum = skinVertexBoneCount[i];
		//		printf("relatedBoneNum\n");
		//		printf("%i\n", relatedBoneNum);
		// set related bone number
		vertex->initPairs(relatedBoneNum);
		/* pair is a struct that stores the related bones of the
		vertex and their power */
		// for each pair
		for(int j = 0; j < relatedBoneNum; j++){
			// get related bone name from Fcollada
			std::string tempboneName = skinVertexBoneName[i][j];
			char* boneName = (char*)tempboneName.c_str();
			//			printf("boneName\n");
			//			printf("%s\n", boneName);
			// get power of the bone to this vertex
			// please notice that the sum of all the powers of a vertex
			// should be 1
			float power = skinVertexBonePower[i][j];
			//			printf("power\n");
			//			printf("%f\n", power);
			// set pair
			vertex->initPair(j,boneName,power);
		}
	}

	/* step4: initialize skeleton and skin instance */
	// this step is easy, just use the function below
	character->initInstance();

	/* step5: initialize VBOs(a VBO is a group of triangles that have the
	same material and textures) */
	// get number of VBOs that you need to display this CharacterObject
	int vboNum = charactervboNum;
	// set vbo number
	character->initVBOs(vboNum);
	// for each vbo
	for(int i = 0; i < vboNum; i++){
		/* vertices is an array of ids of ChVertex that is contained 
		in this VBO, you can get the id of each vertex use getId()
		function of ChVertex, perhaps you can generate all vertices'
		id information when you built the skin */
		int* vertices;
		int vSize = 0;
		// generate each vbo's vertices
		// get the length of vertices
		if( i != vboNum -1){
			vertices = getSkinVertexIndex(skinPolygonIndex[i], skinPolygonIndex[i+1]);
			vSize = skinPolygonIndex[i+1] - skinPolygonIndex[i];
		}
		else{
			vertices = getSkinVertexIndex(skinPolygonIndex[i], skinVertexNum);
			vSize = skinVertexNum - skinPolygonIndex[i];
		}

		// initialize i-th vbo
		character->initVBO(vertices,vSize,i);
		// get texture coordinates of vertices
		float* texCoord = character_vbo_texcoords[i];
		// get length of texCoord
		int tcSize = vSize * 2;
		//		printf("%i\n", tcSize);
		//		printf("\n");
		// get texture id
		GLuint texId = character_vbo_texid[i];
		//		printf("%i\n", texId);
		//		printf("\n");
		// set i-th vbo's texture
		character->setVBOTexture(texCoord,tcSize,texId,i);
		// get material information of that vbo
		float am[4] = {character_vbo_am[i][0], character_vbo_am[i][1], character_vbo_am[i][2], character_vbo_am[i][3]};
		float di[4] = {character_vbo_di[i][0], character_vbo_di[i][1], character_vbo_di[i][2], character_vbo_di[i][3]};
		float sp[4] = {character_vbo_sp[i][0], character_vbo_sp[i][1], character_vbo_sp[i][2], character_vbo_sp[i][3]};
		float em[4] = {character_vbo_em[i][0], character_vbo_em[i][1], character_vbo_em[i][2], character_vbo_em[i][3]};
		float sh = character_vbo_sh[i];
		//		printf("am = (%f,%f,%f,%f)\n", am[0], am[1], am[2], am[3]);
		//		printf("di = (%f,%f,%f,%f)\n", di[0], di[1], di[2], di[3]);
		//		printf("sp = (%f,%f,%f,%f)\n", sp[0], sp[1], sp[2], sp[3]);
		//		printf("em = (%f,%f,%f,%f)\n", em[0], em[1], em[2], em[3]);
		//		printf("sh = %f\n", sh);
		//		printf("\n");
		// set i-th vbo's material
		character->setVBOMaterial(am,di,sp,em,sh,i);
	}

	/* step6: add the character to the ObjectManager */
	objectManager->addVBOObject(character);

}


//set the index of the vertex in the vertex lib in my struct
int * FColladaModelLoader::getSkinVertexIndex(int begin, int end){
	assert(begin != end);
	// init the length of the result, the length is end - begin
	int * tempSkinVertexIndex = new int[end-begin];

	for(int i=0; i<end - begin; i++)
	{
		//set the index value
		tempSkinVertexIndex[i] = begin + i;
	}

	return tempSkinVertexIndex;
}


//store the vertices, normals, texturecoords and create staticobjects to display
void FColladaModelLoader::storeVertices(int kind, FCDocument* m_document)
{
	// how many geometries there are?
	FCDGeometryLibrary* geolib=m_document->GetGeometryLibrary();

	// count the number of meshes (nurbs and splines are included in geolib, so descard them)
	int m_num_geometries=0;
	for (int i=0; i< (int) geolib->GetEntityCount(); i++)
	{
		if (geolib->GetEntity(i)->IsMesh()==true) 
		{
			m_num_geometries++;
		}
	}
	m_ptrs_geometries.reserve(m_num_geometries);

	// copy all geometry objects from fcollada to my structures
	FCDGeometry* geo;
	for (int j=0; j<(int) geolib->GetEntityCount(); j++) { // REWVISAR hacer algo como m_num_meshse
		geo = geolib->GetEntity(j);

		// there are 3 types of geometries, nurbs, splines and meshes
		// at the moment JUST meshes, neither nurbs nor splines
		// meshes
		if (geo->IsMesh()) {
			FCDGeometryMesh* mesh=geo->GetMesh();

			// triangulate this mesh if it was not triangulated
			if (!mesh->IsTriangles()) 
			{
				FCDGeometryPolygonsTools::Triangulate(mesh);
			}

			// create my own polygons
			int m_num_polygons=(int) mesh->GetPolygonsCount();

			for (int p=0;p<m_num_polygons;p++) {
				FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(p);
				FCDGeometrySource* source;
				FCDGeometryPolygonsInput* geometrypolygonsinput;
				uint32* indices;
				// indices to vertex
				geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);
				indices = geometrypolygonsinput->GetIndices();

				//m_num_vertices=(int) indices->size();
				int m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();
				/*store the vertex*/

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::POSITION);

				// allocate memory for triangles and its vertex (a triangle has 3 vertex)
				Vector3D *m_ptrs_vertices = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

				// look for vertices using indices
				// 3 vertex form a triangle

				for (int i=0; i<m_num_vertices; i++) {
					// a vertex index
					int index=(int) indices[i];
					// a vertex values from it index
					float *p=&source->GetData()[index*3];
					m_ptrs_vertices[i].x=p[0];
					m_ptrs_vertices[i].y=p[1];
					m_ptrs_vertices[i].z=p[2];
				}

				/*store the normal*/

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::NORMAL);
				bool m_has_normals;
				Vector3D *m_ptrs_normals;
				if (source==NULL) {
					m_has_normals=false;
				} else {
					m_has_normals=true;
					// indices
					geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::NORMAL);
					indices = geometrypolygonsinput->GetIndices();

					// allocate memory for triangles and its vertex (a triangle has 3 vertex)
					m_ptrs_normals = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

					// look for vertices using indices
					for (int i=0; i<m_num_vertices; i++) {
						// a vertex index
						int index=(int) indices[i];
						// a vertex values from it index
						float *p=&source->GetData()[index*3];
						m_ptrs_normals[i].x=p[0];
						m_ptrs_normals[i].y=p[1];
						m_ptrs_normals[i].z=p[2];
					}
				}

				/*store the texture coordinates*/

				// do the same for texcture coordinates
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::TEXCOORD);
				bool m_has_texcoords;
				Vector3D *m_ptrs_texcoords;
				// maybe they are not present
				if (source==NULL) {
					m_has_texcoords=false;
				} else {
					m_has_texcoords=true;

					// indices
					geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::TEXCOORD);
					indices = geometrypolygonsinput->GetIndices();

					// allocate memory for triangles and its vertex (a triangle has 3 vertex)
					m_ptrs_texcoords = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );
					int stride;
					stride=source->GetStride();

					// 3ds max exports textures with 3 coordinates, but maya and Google Earth use 2 coordinates

					// look for vertices using indices
					for (int i=0; i<m_num_vertices; i++) {
						// a vertex index
						int index=(int) indices[i];
						// a vertex values from it index
						float *p=&source->GetData()[index*stride];
						m_ptrs_texcoords[i].x=p[0];
						m_ptrs_texcoords[i].y=p[1];
						if (stride==3) {
							m_ptrs_texcoords[i].z=p[2]; 
						}
						else{
							m_ptrs_texcoords[i].z=0.0f;
						}
					}
				}

				//init the total data of the vertexs and normals
				float * m_vbo_vertices = new GLfloat[m_num_vertices * 3];
				float * m_vbo_normals = new GLfloat[m_num_vertices * 3];
				float * m_vbo_texcoords = new GLfloat[m_num_vertices * 2];

				//convert the Vector3D vertexs data to my structures 
				for(int i=0; i<m_num_vertices; i++)
				{
					m_vbo_vertices[3*i] = (m_ptrs_vertices+i)->x;
					m_vbo_vertices[3*i+1] = (m_ptrs_vertices+i)->y;
					m_vbo_vertices[3*i+2] = (m_ptrs_vertices+i)->z;
				}
				if(m_has_normals == true)
				{
					//convert the Vector3D normals data to my structures 
					for(int i=0; i<m_num_vertices; i++)
					{
						m_vbo_normals[3*i] = (m_ptrs_normals+i)->x;
						m_vbo_normals[3*i+1] = (m_ptrs_normals+i)->y;
						m_vbo_normals[3*i+2] = (m_ptrs_normals+i)->z;
					}
				}
				if(m_has_texcoords == true)
				{
					//convert the Vector3D texcoords data to my structures 
					for(int i=0; i<m_num_vertices; i++)
					{
						m_vbo_texcoords[2*i] = (m_ptrs_texcoords+i)->x;
						m_vbo_texcoords[2*i+1] = (m_ptrs_texcoords+i)->y;
					}
				}
				//set the textruecoords of the static object
				m_total_texcoords.push_back(m_vbo_texcoords);

				m_size_texcoords.push_back(m_num_vertices * 2);

				if(kind == 2)
				{
					//create the static object with vertex
					StaticObject* objects = new StaticObject(m_vbo_vertices, m_num_vertices * 3);

					//set the normals of the static object
					objects->setNormals(m_vbo_normals, m_num_vertices * 3);

					//add the objects to object manage
					objectManager->addVBOObject(objects);
				}
			}
			m_ptrs_geometries.push_back(mesh);
		}
	}
}





//store all the textures that contains in the document
void FColladaModelLoader::storeTexture(FCDocument* m_document){
	//get the image from the document
	FCDImageLibrary* imagelib=m_document->GetImageLibrary();
	//get the number of the image
	m_num_textures=(int) imagelib->GetEntityCount();
	// copy textures to my structures
	FCDImage* image;
	for (int i=0; i<m_num_textures; i++) {
		image = imagelib->GetEntity(i);
		m_ptrs_textures.push_back(image);
	}
}

//store all the materials that contains in the document
void FColladaModelLoader::storeMaterials(FCDocument* m_document){
	//get the material from the document
	FCDMaterialLibrary* materiallib=m_document->GetMaterialLibrary();
	//get the number of the material
	m_num_materials=(int) materiallib->GetEntityCount();
	m_ptrs_materials.reserve(m_num_materials);

	// copy lights to my structures	
	FCDMaterial* material;

	for (int i=0; i<m_num_materials; i++) {
		material = materiallib->GetEntity(i);
		m_ptrs_materials.push_back(material);
	}
}



//build the scene include the material of the polygons, the texture of the polygons and the matrix of the bone.
//first pass in a set of root node, then look through all the scene node by find the scene node's child node
//for every set of root node, 
//load the instance from the first root node, then store the information of the instance to my struct
//load the right matrix information and the animation information from the set of the node, then 
//store a set of animation matrix for the set of node, a position matrix for the whole scene node
//load the bone information from the scene node. 
void FColladaModelLoader::buildScene(FCDSceneNode** node_origin, int kind)
{
	//child scene node
	FCDSceneNode** child_origin = new FCDSceneNode*[szFlieNum];

	//build the scene instance include the skin information and the material and textrue information
	buildSceneInstance(node_origin[0], kind);

	//build the scene matrix, include the bone position information and animation information
	for(int i = 0; i < szFlieNum; i++){
		buildSceneMatrix(node_origin[i], i);
	}

	//build the bone scene, include the bone information
	initBoneScene(node_origin[0]);

	//build the child node
	for (int i=0; i<(int)node_origin[0]->GetChildrenCount(); i++) {
		for(int j = 0; j < szFlieNum; j++)
		{
			child_origin[j]=node_origin[j]->GetChild(i);
		}
		buildScene(child_origin, kind);
	}
}

//store the information about root bone and joint bone.
// the information include the parent name, children number and children name
//the value contains boneParentName, boneChildName and boneChildNum.
void FColladaModelLoader::initBoneScene(FCDSceneNode* node_origin){
	//check if the node is a joint
	if(node_origin ->GetJointFlag() == true){
		//check if the joint is root scene
		if(isBootBoneSceneNode == true)
		{
			//get the root bone child count
			int boneChildCount = 0;
			for(int i = 0; i< (int)node_origin->GetChildrenCount(); i++)
			{
				//check if the child's name is in my struct. 
				int tempChildIndex= getBoneIndexByName(node_origin->GetChild(i)->GetSubId().c_str());

				//count the bone child which we need
				if(tempChildIndex >= 0 && tempChildIndex< boneNumber){
					if(node_origin->GetChild(i)->GetJointFlag() ==true)
						boneChildCount++;
				}
			}	
			//store to my struct
			rootBoneChildNum = boneChildCount;

			//get the root bone child name
			//get the bone child name
			int tempChildNum = rootBoneChildNum;
			rootBoneChildName = new std::string[tempChildNum];

			int tempChildCount = 0;
			for(int i = 0; i< (int)node_origin->GetChildrenCount(); i++){
				//check if the child is a joint
				if(node_origin->GetChild(i)->GetJointFlag() ==true){
					//look for the bone name in my struct 
					int tempChildIndex= getBoneIndexByName(node_origin->GetChild(i)->GetSubId().c_str());
					if(tempChildIndex >= 0 && tempChildIndex< boneNumber){
						//store the root bone name
						rootBoneChildName[tempChildCount] = boneName[tempChildIndex];
						tempChildCount++;
					}
				}
			}
		}
		//set the flag, so the first node is the root scene node
		isBootBoneSceneNode =false;

		//get the index of the joint node by the name 
		int index = getBoneIndexByName(node_origin->GetSubId().c_str());

		//check if the name is in my struct 
		if(index >= 0 && index< boneNumber)
		{
			//check if the node has a parent
			if(node_origin->GetParentCount() > 0){
				//get the index of the parent node
				int parentIndex = getBoneIndexByName(node_origin->GetParent(0)->GetSubId().c_str());

				//check whether the parent node is in my struct
				if(parentIndex >= 0 && parentIndex< boneNumber)
				{
					boneParentName[index] = boneName[parentIndex];
				}

				//set the parent to "", if the parent is not in my struct
				else{
					boneParentName[index] = "";
				}
			}

			//set the parent to "", if the node has no parent
			else{
				boneParentName[index] = "";
			}

			//get the bone child num
			int boneChildCount = 0;
			for(int i = 0; i< (int)node_origin->GetChildrenCount(); i++)
			{
				//find the index of the child node by the name
				int tempChildIndex= getBoneIndexByName(node_origin->GetChild(i)->GetSubId().c_str());
				if(tempChildIndex >= 0 && tempChildIndex< boneNumber){
					if(node_origin->GetChild(i)->GetJointFlag() ==true)
						boneChildCount++;
				}
			}
			
			//store to my struct
			boneChildNum[index] = boneChildCount;

			//get the bone child name
			int tempChildNum = boneChildNum[index];
			boneChildName[index] = new std::string[tempChildNum];
			int tempChildCount = 0;
			for(int i = 0; i< (int)node_origin->GetChildrenCount(); i++){
				//check whether the child is a joint
				if(node_origin->GetChild(i)->GetJointFlag() ==true){
					//get the index of the child by the name
					int tempChildIndex= getBoneIndexByName(node_origin->GetChild(i)->GetSubId().c_str());

					//store the child name to my struct
					if(tempChildIndex >= 0 && tempChildIndex< boneNumber){
						boneChildName[index][tempChildCount] = boneName[tempChildIndex];
						tempChildCount++;
					}
				}
			}
		}
	}
}



//search the index of name in the boneName value , return -1, if can't find the name in it
int FColladaModelLoader::getBoneIndexByName(std::string name)
{
	for(int i=0; i<boneNumber; i++)
	{
		if(boneName[i]==name)
			return i;
	}
	return -1;
}


//set the skin instance information and geo instance information
//contains the skin information, weight information, the vbo information
//it set the value: skinPolygonIndex, skinVertexNum, skinVertexPosX,
//skinVertexPosY, skinVertexPosZ, skinNormalPosX, skinNormalPosY, skinNormalPosZ,
//skinVertexBoneCount, skinPolygonVertexIndex, skinVertexBoneName, skinVertexBonePower,
//it init the value charactervboNum, character_vbo_vertices, character_vbo_normals, character_vbo_texcoords,
//character_vbo_am, character_vbo_di, character_vbo_sp, character_vbo_em, character_vbo_sh, character_vbo_texid,
void FColladaModelLoader::buildSceneInstance(FCDSceneNode* node_origin, int kind)
{
	// copy node instances
	FCDEntityInstance* instance;

	//the entity of the instance, and the name of the entity
	FCDEntity* entity;
	std::string name;

	//instance flag to check which the instance id found in the library
	bool flag_found;

	//look through the instance
	for (int i=0; i<(int)node_origin->GetInstanceCount(); i++) {

		//set or reset the parameter
		flag_found=false;
		instance=node_origin->GetInstance(i);
		entity=instance->GetEntity();
		name=entity->GetDaeId().c_str();

		//check if the instance is controller instance
		if(instance->GetType()==FCDEntityInstance::CONTROLLER)
		{
			//get the controller instance
			FCDControllerInstance* controllerInstance = dynamic_cast<FCDControllerInstance*>(instance);

			//get the skin controller
			FCDSkinController* skin = dynamic_cast<FCDController*>(controllerInstance->GetEntity())->GetSkinController();

			isBootBoneSceneNode =true;

			//build the skin
			buildSkin(skin);
			// look for this name in geo library
			for (int j=0; j<(int)m_ptrs_geometries.size(); j++) {

				//get the target geometries
				if (m_ptrs_geometries[j]->GetDaeId().c_str()==skin->GetTarget()->GetDaeId().c_str()) {				
					//get the polygon index
					int meshIndex=0;
					for(int p=0; p<j; p++)
					{
						meshIndex+=(int)m_ptrs_geometries[p]->GetPolygonsCount();
					}

					//get the geometry mesh
					FCDGeometryMesh* mesh=m_ptrs_geometries[j];

					// create my own polygons
					int m_num_polygons=(int) mesh->GetPolygonsCount();

					//init vbo value
					charactervboNum = m_num_polygons;
					character_vbo_vertices = new float*[charactervboNum];
					character_vbo_normals = new float*[charactervboNum];
					character_vbo_texcoords = new float*[charactervboNum];
					character_vbo_am = new float*[charactervboNum];
					character_vbo_di = new float*[charactervboNum];
					character_vbo_sp = new float*[charactervboNum];
					character_vbo_em = new float*[charactervboNum];
					character_vbo_sh = new float[charactervboNum];
					character_vbo_texid = new GLuint[charactervboNum];

					//temp value for calculating the skin polygon index
					int tempSkinPolygonIndexCount = 0;

					//temp value for setting the size
					int skinPolygonTotalVertexNum = 0;

					for (int p=0;p<m_num_polygons;p++) {
						FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(p);
						FCDGeometryPolygonsInput* geometrypolygonsinput;
						// indices to vertex
						geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);

						//m_num_vertices=(int) indices->size();
						int m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();
						skinPolygonTotalVertexNum += m_num_vertices;
					}
					
					//init the skin value
					skinPolygonIndex = new int[m_num_polygons];
					skinVertexNum = skinPolygonTotalVertexNum;
					skinVertexPosX = new float[skinVertexNum];
					skinVertexPosY = new float[skinVertexNum];
					skinVertexPosZ = new float[skinVertexNum];
					skinNormalPosX = new float[skinVertexNum];
					skinNormalPosY = new float[skinVertexNum];
					skinNormalPosZ = new float[skinVertexNum];
					skinVertexBoneCount = new int[skinVertexNum];
					skinPolygonVertexIndex = new int[skinVertexNum];
					skinVertexBoneName = new std::string*[skinVertexNum];
					skinVertexBonePower = new float*[skinVertexNum];

					//temp value for building the vbo object
					int tempPolygonVertexIndex = 0;

					for (int p=0;p<m_num_polygons;p++) {
						FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(p);
						FCDGeometrySource* source;
						FCDGeometryPolygonsInput* geometrypolygonsinput;
						uint32* indices;
						// indices to vertex
						geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);
						indices = geometrypolygonsinput->GetIndices();

						//m_num_vertices=(int) indices->size();
						int m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();
						/*store the vertex*/

						// source of vertex
						source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::POSITION);

						// allocate memory for triangles and its vertex (a triangle has 3 vertex)
						Vector3D *m_ptrs_vertices = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

						// look for vertices using indices
						// 3 vertex form a triangle

						for (int im=0; im<m_num_vertices; im++) {
							// a vertex index
							int index=(int) indices[im];
							skinPolygonVertexIndex[tempPolygonVertexIndex+im] = index;
							// a vertex values from it index
							float *p=&source->GetData()[index*3];
							m_ptrs_vertices[im].x=p[0];
							m_ptrs_vertices[im].y=p[1];
							m_ptrs_vertices[im].z=p[2];
							//store the skin vertex position
							skinVertexPosX[tempPolygonVertexIndex+im] = p[0];
							skinVertexPosY[tempPolygonVertexIndex+im] = p[1];
							skinVertexPosZ[tempPolygonVertexIndex+im] = p[2];
						}

						/*store the normal*/

						// source of vertex
						source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::NORMAL);
						bool m_has_normals;
						Vector3D *m_ptrs_normals;
						if (source==NULL) {
							m_has_normals=false;
						} else {
							m_has_normals=true;
							// indices
							geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::NORMAL);
							indices = geometrypolygonsinput->GetIndices();

							// allocate memory for triangles and its vertex (a triangle has 3 vertex)
							m_ptrs_normals = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

							// look for vertices using indices
							for (int im=0; im<m_num_vertices; im++) {
								// a vertex index
								int index=(int) indices[im];
								// a vertex values from it index
								float *p=&source->GetData()[index*3];
								m_ptrs_normals[im].x=p[0];
								m_ptrs_normals[im].y=p[1];
								m_ptrs_normals[im].z=p[2];
								//store the skin normal value
								skinNormalPosX[tempPolygonVertexIndex+im] = p[0];
								skinNormalPosY[tempPolygonVertexIndex+im] = p[1];
								skinNormalPosZ[tempPolygonVertexIndex+im] = p[2];
							}
						}

						/*store the texture coordinates*/

						// do the same for texcture coordinates
						source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::TEXCOORD);
						bool m_has_texcoords;
						Vector3D *m_ptrs_texcoords;
						// maybe they are not present
						if (source==NULL) {
							m_has_texcoords=false;
						} else {
							m_has_texcoords=true;

							// indices
							geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::TEXCOORD);
							indices = geometrypolygonsinput->GetIndices();

							// allocate memory for triangles and its vertex (a triangle has 3 vertex)
							m_ptrs_texcoords = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );
							int stride;
							stride=source->GetStride();

							// 3ds max exports textures with 3 coordinates, but maya and Google Earth use 2 coordinates

							// look for vertices using indices
							for (int im=0; im<m_num_vertices; im++) {
								// a vertex index
								int index=(int) indices[im];
								// a vertex values from it index
								float *p=&source->GetData()[index*stride];
								m_ptrs_texcoords[im].x=p[0];
								m_ptrs_texcoords[im].y=p[1];
								if (stride==3) {
									m_ptrs_texcoords[im].z=p[2]; 
								}
								else{
									m_ptrs_texcoords[im].z=0.0f;
								}
							}
						}
						//init the total data of the vertexs and normals
						float * m_vbo_vertices = new float[m_num_vertices * 3];
						float * m_vbo_normals = new float[m_num_vertices * 3];
						float * m_vbo_texcoords = new float[m_num_vertices * 2];

						//convert the Vector3D vertexs data to my structures 
						for(int im=0; im<m_num_vertices; im++)
						{
							m_vbo_vertices[3*im] = (m_ptrs_vertices+im)->x;
							m_vbo_vertices[3*im+1] = (m_ptrs_vertices+im)->y;
							m_vbo_vertices[3*im+2] = (m_ptrs_vertices+im)->z;
						}
						if(m_has_normals == true)
						{
							//convert the Vector3D normals data to my structures 
							for(int im=0; im<m_num_vertices; im++)
							{
								m_vbo_normals[3*im] = (m_ptrs_normals+im)->x;
								m_vbo_normals[3*im+1] = (m_ptrs_normals+im)->y;
								m_vbo_normals[3*im+2] = (m_ptrs_normals+im)->z;
							}
						}
						if(m_has_texcoords == true)
						{
							//convert the Vector3D texcoords data to my structures 
							for(int im=0; im<m_num_vertices; im++)
							{
								m_vbo_texcoords[2*im] = (m_ptrs_texcoords+im)->x;
								m_vbo_texcoords[2*im+1] = (m_ptrs_texcoords+im)->y;
							}
						}

						//calculate the index information for vbo, and store to my struct
						
						//get the size of the vertex, normal and texcoord
						character_vbo_size = m_num_vertices;
						character_vbo_vertices[p] = m_vbo_vertices;
						character_vbo_normals[p] = m_vbo_normals;
						character_vbo_texcoords[p] = m_vbo_texcoords;
						
						//count the temp value for storing the vbo
						tempPolygonVertexIndex += m_num_vertices;
						
						//get the index of the polygon for calculating the vertex index
						skinPolygonIndex[p] = tempSkinPolygonIndexCount;
						
						//count the number to get the right skin polygon index
						tempSkinPolygonIndexCount += m_num_vertices;
					}
					
					//set the influences of the vertex pair
					for (int i= 0; i<skinVertexNum; i++)
					{
						//get the influence struct
						FCDSkinControllerVertex* influences = skin->GetVertexInfluence(skinPolygonVertexIndex[i]);
						//store the bone count
						skinVertexBoneCount[i] = (int)influences->GetPairCount();
						//init the skin bone name
						skinVertexBoneName[i] = new std::string[skinVertexBoneCount[i]];
						//store the bone power
						skinVertexBonePower[i] = new float[skinVertexBoneCount[i]];
						for(int j =0; j < (int)influences->GetPairCount(); j++)
						{
							//get the weight pair struct
							FCDJointWeightPair* pairs = influences->GetPair(j);
							//store the skin bone name
							skinVertexBoneName[i][j] = boneName[(int)pairs->jointIndex];
							//store the skin bone power
							skinVertexBonePower[i][j] = pairs->weight;
						}
					}

					FCDMaterial* material;

					//the id of the pointed material
					std::string id_material;

					//look through the mesh
					for (int i=0; i<(int)mesh->GetPolygonsCount(); i++) {

						material=NULL;
						// look for this material_semantic in geometry_instance
						for (int k=0; k<(int)controllerInstance->GetMaterialInstanceCount(); k++) {
							// look for this material in my material lib

							FCDMaterialInstance* materialInstance = controllerInstance->GetMaterialInstance(k);
							//get the material instance from the geo instance

							//check whether the semantic of the material instance is equals to the semantic of the polygon 
							if (materialInstance->GetSemantic()==mesh->GetPolygons(i)->GetMaterialSemantic()) {
								//get the material id of the pointed material
								id_material=materialInstance->GetMaterial()->GetDaeId().c_str();

								// look for a pointer for that material in my material lib
								for (int j=0; j<m_num_materials; j++){
									if (m_ptrs_materials[j]->GetDaeId().c_str()==id_material) {
										//set the material of the polygon
										setFCMaterial(j, meshIndex+i, i, kind);
									}
								}
							}
						}
					}
				}
			}
		}
		if(kind == 2)
		{
			// look for this name in geo library
			for (int j=0; j<(int)m_ptrs_geometries.size(); j++) {
				if (m_ptrs_geometries[j]->GetDaeId().c_str()==name) {
					//get the Geo instance
					FCDGeometryInstance* geometry_instance=dynamic_cast<FCDGeometryInstance*>(instance);
					//get the mesh
					FCDGeometryMesh* mesh=m_ptrs_geometries[j];
					//get the polygon index
					int meshIndex=0;
					for(int p=0; p<j; p++)
					{
						meshIndex+=(int)m_ptrs_geometries[p]->GetPolygonsCount();
					}
					//get the material
					setMeshFCMaterial(geometry_instance, mesh, meshIndex, kind);
					flag_found=true;
					break;
				}
			}
			if (flag_found) continue;
		}
	}
}

//build the bone information, the inverse information
//it set the value boneNumber boneName boneMatrix boneInverseMatrix boneParentName 
//boneChildNum boneChildName
//it init the value animationsBoneFrameNum animationsBoneFrameMatrix animationsBoneFrameTime.
void FColladaModelLoader::buildSkin(FCDSkinController* skin){
	boneNumber = (int)skin->GetJointCount();
	boneName = new std::string[boneNumber];
	boneMatrix = new Matrix[boneNumber];
	boneInverseMatrix = new Matrix[boneNumber];
	boneParentName = new std::string[boneNumber];
	boneChildNum = new int[boneNumber];
	boneChildName = new std::string*[boneNumber];

	//init the animation bone informaiton
	for(int i = 0; i < szFlieNum; i++){
		animationsBoneFrameNum[i] = new int[boneNumber];
		animationsBoneFrameMatrix[i] = new Matrix*[boneNumber];
		animationsBoneFrameTime[i] = new int*[boneNumber];
	}

	//look through the joint
	for(int i = 0; i < (int)skin->GetJointCount(); i++)
	{
		//store the bone name
		FCDSkinControllerJoint* joint = skin->GetJoint(i);
		boneName[i] = joint->GetId().c_str();

		//store the inverse of the joint
		FMMatrix44 inverse = joint->GetBindPoseInverse() * skin->GetBindShapeTransform();
		float tempMatrixElement[] ={inverse.m[0][0], inverse.m[0][1], inverse.m[0][2], inverse.m[0][3], 
			inverse.m[1][0], inverse.m[1][1], inverse.m[1][2], inverse.m[1][3],
			inverse.m[2][0], inverse.m[2][1], inverse.m[2][2], inverse.m[2][3],
			inverse.m[3][0], inverse.m[3][1], inverse.m[3][2], inverse.m[3][3],};
		Matrix tempMatrix = Matrix(tempMatrixElement);
		boneInverseMatrix[i] = tempMatrix;

	}
}



//build the matrix of the node, include the matrix for the node and the animation information
//it store the value animationsBoneFrameNum animationsBoneFrameMatrix animationsBoneFrameTime
//rootAnimationsBoneFrameNum rootAnimationsBoneFrameMatrix rootAnimationsBoneFrameTime;
void FColladaModelLoader::buildSceneMatrix(FCDSceneNode* node_origin, int m_documentIndex)
{
	//check if the node is a joint
	if(node_origin ->GetJointFlag() == true){
		//get the index of the bone
		int index = getBoneIndexByName(node_origin->GetSubId().c_str());

		//check if the index is available
		if(index>=0 && index< boneNumber){
			//init the value animationsBoneFrameNum
			animationsBoneFrameNum[m_documentIndex][index] = 0;
		}

		//check if the node is root bone 
		if(isRootBoneName[m_documentIndex] == true)
		{
			//store the root bone scene node
			FCDSceneNode* rootBoneSceneNode = node_origin;

			//store the root bone matrix
			FMMatrix44 inverse = node_origin->ToMatrix();
			rootBoneMatrix = convertToMatrix(inverse);

			FCDTransform*  trans_origin;
			for (int i=0; i<(int)node_origin->GetTransformCount(); i++) {
				//get the transform struct
				trans_origin=node_origin->GetTransform(i);
				// matrix
				if (trans_origin->GetType()== FCDTransform::MATRIX) {
					FCDTMatrix* trans_matrix=dynamic_cast<FCDTMatrix*>(trans_origin); // casting

					// animation
					FCDAnimated* animated;
					//int num_curves;
					if (trans_matrix->IsAnimated()) {		
						//get the animated struct
						animated=trans_matrix->GetAnimated();
						// get first animation curve id
						unsigned int firstCurveId = 0;
						while(firstCurveId<animated->GetValueCount() && animated->GetCurve(firstCurveId)==NULL)
							firstCurveId++;
						
						if(firstCurveId < animated->GetValueCount()){
							// has animation
							int animationKeyCount = animated->GetCurve(firstCurveId)->GetKeyCount();

							//store the frame number, init the matrix and time information
							rootAnimationsBoneFrameNum[m_documentIndex] = animationKeyCount;
							rootAnimationsBoneFrameMatrix[m_documentIndex] = new Matrix[animationKeyCount];
							rootAnimationsBoneFrameTime[m_documentIndex] = new int[animationKeyCount];
							float tempAnimationMatrixFloat[16];
							for(int j = 0; j < animationKeyCount; j++)
							{					
								for(int p = 0; p < 16; p++)
								{
									FCDAnimationCurve * curve =animated->GetCurve(p);
									if(curve == NULL){
										tempAnimationMatrixFloat[p] = int(p%5 == 0); 
										continue;
									}
									//store the frame time information
									rootAnimationsBoneFrameTime[m_documentIndex][j] = (int)(curve->GetKey(j)->input * sigle_frame_time + 0.5);
									tempAnimationMatrixFloat[p] = curve->GetKey(j)->output;
								}

								FMMatrix44 fmmatrix(tempAnimationMatrixFloat);

								//store the matrix information
								Matrix tempAnimationMatrix = convertToMatrix(fmmatrix);
								rootAnimationsBoneFrameMatrix[m_documentIndex][j] = tempAnimationMatrix;
							}
						}
						else {
							// no animation
							rootAnimationsBoneFrameNum[m_documentIndex] = 0;
						}
					}
				}
			}
		}
		isRootBoneName[m_documentIndex] = false;

		//check if the node is the bone in the skin
		if(index >= 0 && index< boneNumber)
		{
			//store the bone matrix
			FMMatrix44 inverse = node_origin->ToMatrix();

			boneMatrix[index] = convertToMatrix(inverse);

			FCDTransform*  trans_origin;

			// copy node transformations
			for (int i=0; i<(int)node_origin->GetTransformCount(); i++) {
				trans_origin=node_origin->GetTransform(i);
				// matrix
				if (trans_origin->GetType()== FCDTransform::MATRIX) {
					FCDTMatrix* trans_matrix=dynamic_cast<FCDTMatrix*>(trans_origin); // casting

					// animation
					FCDAnimated* animated;
					//int num_curves;
					if (trans_matrix->IsAnimated()) {		
						animated=trans_matrix->GetAnimated();
						assert((int)animated->GetValueCount() == 16);
						int animationKeyCount = animated->GetCurves().at(0)[0]->GetKeyCount();
						//store the frame number, init the matrix and time information
						animationsBoneFrameNum[m_documentIndex][index] = animationKeyCount;
						animationsBoneFrameMatrix[m_documentIndex][index] = new Matrix[animationKeyCount];
						animationsBoneFrameTime[m_documentIndex][index] = new int[animationKeyCount];

						for(int j = 0; j < animationKeyCount; j++)
						{
							//store the frame time
							animationsBoneFrameTime[m_documentIndex][index][j] = (int)((animated->GetCurves().at(0)[0]->GetKey(j)->input * sigle_frame_time) + 0.5);	
							float* tempAnimationMatrixFloat = new float[(int)animated->GetValueCount()];						
							for(int p = 0; p < (int)animated->GetValueCount(); p++)
							{
								tempAnimationMatrixFloat[p] = animated->GetCurves().at(p)[0]->GetKey(j)->output;
							}

							//store the matrix
							FMMatrix44 fmmatrix(tempAnimationMatrixFloat);

							Matrix tempAnimationMatrix = convertToMatrix(fmmatrix);
							animationsBoneFrameMatrix[m_documentIndex][index][j] = tempAnimationMatrix;
						}

					}
					continue;
				}
			}
		}
	}
}
//set the material of the mesh
void FColladaModelLoader::setMeshFCMaterial(FCDGeometryInstance* geometry_instance, FCDGeometryMesh* mesh, int meshIndex, int kind)
{
	// fill this mesh instance with polygons instances

	//the id of the pointed material
	std::string id_material;

	//material and material instance
	FCDMaterial* material;
	FCDMaterialInstance* materialinstance;

	//look through the mesh
	for (int i=0; i<(int)mesh->GetPolygonsCount(); i++) {

		material=NULL;
		// look for this material_semantic in geometry_instance
		for (int k=0; k<(int)geometry_instance->GetMaterialInstanceCount(); k++) {
			// look for this material in my material lib

			//get the material instance from the geo instance
			materialinstance=geometry_instance->GetMaterialInstance(k);

			//check whether the semantic of the material instance is equals to the semantic of the polygon 
			if (materialinstance->GetSemantic()==mesh->GetPolygons(i)->GetMaterialSemantic()) {
				//get the material id of the pointed material
				id_material=materialinstance->GetMaterial()->GetDaeId().c_str();

				// look for a pointer for that material in my material lib
				for (int j=0; j<m_num_materials; j++){
					if (m_ptrs_materials[j]->GetDaeId().c_str()==id_material) {
						//set the material of the polygon
						setFCMaterial(j, meshIndex+i, i, kind);
					}
				}
			}
		}
	}
}

//set the target material and texture to the polygon
void FColladaModelLoader::setFCMaterial(int target, int index, int polygonIndex, int kind)
{
	//target material
	FCDMaterial* material = m_ptrs_materials[target];

	// pointer to diffuse texture
	FCDImage* m_texture_diffuse;
	bool m_has_diffuse_texture;

	// pointer to texture reflective
	FCDImage* m_texture_reflective;
	bool m_has_reflective_texture;

	// pointer to texture transparent
	FCDImage* m_texture_transparent;
	bool m_has_transparent_texture;

	// get the material effect and the profile
	FCDEffect* fx = material->GetEffect();
	FCDEffectProfile* profile = fx->FindProfile(FUDaeProfileType::COMMON);

	if (profile==NULL) {
		return;
	}

	// copy properties of common profile
	FCDEffectStandard* standardProfile=dynamic_cast<FCDEffectStandard*>(profile);

	// configure alpha value in diffuse material
	float m_transparency=standardProfile->GetTranslucencyFactor();

	float m_shininess;
	FCVector4D m_emission;
	FCVector4D m_ambient;
	FCVector4D m_diffuse;
	FCVector4D m_specular;

	// ambient
	m_ambient.Set(
		standardProfile->GetAmbientColor().x,
		standardProfile->GetAmbientColor().y,
		standardProfile->GetAmbientColor().z,
		standardProfile->GetAmbientColor().w			  
		);

	// diffuse component
	m_diffuse.Set(
		standardProfile->GetDiffuseColor().x,
		standardProfile->GetDiffuseColor().y,
		standardProfile->GetDiffuseColor().z,
		1.0 // opaque for opengl, use m_transparency on polygon::render 		  
		);

	// specular
	float specular_factor=standardProfile->GetSpecularFactor();
	m_specular.Set(
		specular_factor*standardProfile->GetSpecularColor().x,
		specular_factor*standardProfile->GetSpecularColor().y,
		specular_factor*standardProfile->GetSpecularColor().z,
		specular_factor*standardProfile->GetSpecularColor().w			  
		);

	// shininess
	m_shininess=standardProfile->GetShininess();

	// emission
	if (standardProfile->IsEmissionFactor()) {
		m_emission.Set(
			standardProfile->GetEmissionFactor(),
			standardProfile->GetEmissionFactor(),
			standardProfile->GetEmissionFactor(),
			standardProfile->GetEmissionFactor()
			);
	} else {
		m_emission.Set(
			standardProfile->GetEmissionColor().x*standardProfile->GetEmissionFactor(),
			standardProfile->GetEmissionColor().y*standardProfile->GetEmissionFactor(),
			standardProfile->GetEmissionColor().z*standardProfile->GetEmissionFactor(),
			standardProfile->GetEmissionColor().w*standardProfile->GetEmissionFactor()
			);
	}

	//store the data to my structure
	GLfloat am[4] = {m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ(), m_ambient.GetW()};
	GLfloat di[4] = {m_diffuse.GetX(), m_diffuse.GetY(), m_diffuse.GetZ(), m_diffuse.GetW()};
	GLfloat sp[4] = {m_specular.GetX(), m_specular.GetY(), m_specular.GetZ(), m_specular.GetW()};
	GLfloat em[4] = {m_emission.GetX(), m_emission.GetY(), m_emission.GetZ(), m_emission.GetW()};
	GLfloat sh = (GLfloat)m_shininess;

	/*texture*/
	// textures
	FCDTexture* texture;
	FCDImage *image;

	// diffuse textures
	m_has_diffuse_texture=false;
	m_texture_diffuse=NULL;
	image=NULL;

	// diffuse texture
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::DIFFUSE)>0) {
		texture=standardProfile->GetTexture(FUDaeTextureChannel::DIFFUSE,0);
		if (texture!=NULL) {
			image=texture->GetImage();
			if (image!=NULL) {
				m_texture_diffuse=SearchTextureByName(image->GetDaeId());
				if (m_texture_diffuse!=NULL)
					m_has_diffuse_texture=true;
			}
		}
	}	

	// reflective texture
	m_has_reflective_texture=false;
	m_texture_reflective=NULL;
	image=NULL;

	// reflective texture
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::REFLECTION)>0) {
		texture=standardProfile->GetTexture(FUDaeTextureChannel::REFLECTION,0);
		if (texture!=NULL) {
			image=texture->GetImage();
			if (image!=NULL) {
				m_texture_reflective=SearchTextureByName(image->GetDaeId());
				if (m_texture_reflective!=NULL)
					m_has_reflective_texture=true;
			}
		}
	}

	// transparent textures
	m_has_transparent_texture=false;
	m_texture_transparent=NULL;
	image=NULL;

	// transparent texture
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::TRANSPARENT)>0) {
		texture=standardProfile->GetTexture(FUDaeTextureChannel::TRANSPARENT,0);
		if (texture!=NULL) {
			image=texture->GetImage();
			if (image!=NULL) {
				m_texture_transparent=SearchTextureByName(image->GetDaeId());
				if (m_texture_transparent!=NULL)
					m_has_transparent_texture=true;
			}
		}
	}

	//create the texture manage
	TextureManager* textureManager = TextureManager::getInstance();

	GLuint tex = NULL;

	//diffuse texture
	if(m_texture_diffuse!=NULL)
	{
		//the absolute file path
		char szPath[512];
		//convert unicode to string
		WideCharToMultiByte(CP_ACP, 0, m_texture_diffuse->GetFilename().c_str(), -1, szPath, MAX_PATH, NULL, NULL);	
		//temp path 
		char		tempPath[MAX_PATH+1];		
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, tempPath);		
		//convert char* to string
		std::string tempDir=tempPath;
		//get the size of the string
		int szDirSize = tempDir.size();
		//convert char* to string
		std::string szFile=szPath;
		//get the relative file path
		szFile = szFile.substr(szDirSize+1);

		//get the texture from the relative path
		tex = textureManager->getTextureId(szFile.c_str());
	}

	//reflective texture
	if(m_texture_reflective!=NULL)
	{
		//the absolute file path
		char szPath[512];
		//convert unicode to string
		WideCharToMultiByte(CP_ACP, 0, m_texture_reflective->GetFilename().c_str(), -1, szPath, MAX_PATH, NULL, NULL);	
		//temp path 
		char		tempPath[MAX_PATH+1];		
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, tempPath);		
		//convert char* to string
		std::string tempDir=tempPath;
		//get the size of the string
		int szDirSize = tempDir.size();
		//convert char* to string
		std::string szFile=szPath;
		//get the relative file path
		szFile = szFile.substr(szDirSize+1);

		//get the texture from the relative path
		tex = textureManager->getTextureId(szFile.c_str());
	}

	//transparent texture
	if(m_texture_transparent!=NULL)
	{
		//the absolute file path
		char szPath[512];
		//convert unicode to string
		WideCharToMultiByte(CP_ACP, 0, m_texture_transparent->GetFilename().c_str(), -1, szPath, MAX_PATH, NULL, NULL);	
		//temp path 
		char		tempPath[MAX_PATH+1];		
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, tempPath);		
		//convert char* to string
		std::string tempDir=tempPath;
		//get the size of the string
		int szDirSize = tempDir.size();
		//convert char* to string
		std::string szFile=szPath;
		//get the relative file path
		szFile = szFile.substr(szDirSize+1);
		//get the texture from the relative path
		tex = textureManager->getTextureId(szFile.c_str());
	}

	if(kind == 0)
	{
		//store the vbo material and texture information
		character_vbo_am[polygonIndex] = new float[4];
		character_vbo_di[polygonIndex] = new float[4];
		character_vbo_sp[polygonIndex] = new float[4];
		character_vbo_em[polygonIndex] = new float[4];
		//if has texture, set the texture
		character_vbo_am[polygonIndex][0] = am[0];
		character_vbo_am[polygonIndex][1] = am[1];
		character_vbo_am[polygonIndex][2] = am[2];
		character_vbo_am[polygonIndex][3] = am[3];
		character_vbo_di[polygonIndex][0] = di[0];
		character_vbo_di[polygonIndex][1] = di[1];
		character_vbo_di[polygonIndex][2] = di[2];
		character_vbo_di[polygonIndex][3] = di[3];
		character_vbo_sp[polygonIndex][0] = sp[0];
		character_vbo_sp[polygonIndex][1] = sp[1];
		character_vbo_sp[polygonIndex][2] = sp[2];
		character_vbo_sp[polygonIndex][3] = sp[3];
		character_vbo_em[polygonIndex][0] = em[0];
		character_vbo_em[polygonIndex][1] = em[1];
		character_vbo_em[polygonIndex][2] = em[2];
		character_vbo_em[polygonIndex][3] = em[3];
		character_vbo_sh[polygonIndex] = sh;
		if(tex != NULL)
		{
			character_vbo_texid[polygonIndex] = tex;
		}
	}
	if(kind == 2)
	{
		if(tex != NULL)
		{
			((StaticObject *) objectManager->getVBOObject(2,index))->setTextures(m_total_texcoords[index], m_size_texcoords[index], tex);
		}

		//set the material
		((StaticObject *) objectManager->getVBOObject(2,index))->setMaterial(am, di, sp, em, sh);
	}
}

//search the texture in the texture lib by texture id
FCDImage *FColladaModelLoader::SearchTextureByName(fm::string textureid){

	// look for this texture
	for (int i=0; i<m_num_textures; i++)
	{
		if (m_ptrs_textures[i]->GetDaeId()==textureid) 
		{
			return m_ptrs_textures[i];
		}
	}
	// if fail, null
	return NULL;
}




