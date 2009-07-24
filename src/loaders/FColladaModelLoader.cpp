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
#include "ModelLoader.h"
#include "../managers/TextureManager.h"
#include "FCVector4D.h"
#include "FColladaModelLoader.h"

//constructor
FColladaModelLoader::FColladaModelLoader(void){

	//initialize the Fcollada namespace
	FCollada::Initialize(); 

	//initialize the values
	m_document=NULL;
	boneNumber = 0;
}

//destructor
FColladaModelLoader::~FColladaModelLoader(void){
	delete m_document;
}

//load the model from a path of a fcollada file
//return ture if load success
//set the document value while load the model
//it also contain three methed storeVertices, storeTexture, storeMaterials. which
//store the vertices, texture, materials to my own structure 

bool FColladaModelLoader::loadModel(int kind,const char* szPathName){

	//it is a class value, which administrate the 
	objectManager = ObjectManager::getInstance();
	std::string tempFilename = szPathName;
	int tempFileNameIndex = tempFilename.find_last_of("/");
	filename = tempFilename.substr(tempFileNameIndex+1, tempFilename.length()-5-tempFileNameIndex);
	// new dae file
	m_document = FCollada::NewTopDocument();

	//path in unicode
	OLECHAR		wszPath[MAX_PATH+1];

	//temp path
	char		szPath[MAX_PATH+1];	

	//if the path contains the http:// , then copy the path to the szPath 
	if (strstr(szPathName, "http://"))									
	{
		strcpy(szPath, szPathName);										
	}
	// else load the model from the file
	else																
	{
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, szPath);	

		//add the '\'
		strcat(szPath, "\\");

		//add the relative path
		strcat(szPath, szPathName);										
	}

	//convert ascii to unicode
	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);

	//convert the type of path to fstring
	fstring fpath=FUStringConversion::ToFString(wszPath);

	//load the document from the file. return true, if success
	bool ret=FCollada::LoadDocumentFromFile(m_document, fpath);	

	assert(ret);

	//store the vertices, normals, texturecoords and create staticobjects to display
	storeVertices(kind);

	storeLight();

	//store all the textures that contains in the document
	storeTexture();

	//store all the materials that contains in the document
	storeMaterials();

	storeCamera();

	storeAnimation();

	//build the scene
	FCDSceneNode* ptr_root=m_document->GetVisualSceneRoot();
	if(ptr_root!=NULL)
	{
		buildScene(ptr_root, kind);
		drawLine(ptr_root, kind);
	}

	if(kind == 0)
	{
		BuildCharacter();
	}

	return true;
}

void FColladaModelLoader::BuildCharacter()
{
	// create a new CharacterObject
	CharacterObject* character = new CharacterObject();

	/* step1: initialize skeleton */
	// create a new ChSkeleton
	ChSkeleton* skeleton = character->getSkeleton();
	// get bone number of the skeleton from Fcollada
	int bone_num = 0;

	bone_num = getBoneNumber();
//	printf("boneNumber\n");
//	printf("%i\n", boneNumber);
//	printf("\n");
	// set bone number to skeleton
	skeleton->init(bone_num);
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
			// set root bone
			skeleton->setRootBone(bone);
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
	// calculate tranform in world space
	skeleton->calculateAbsoluteTransform();
//	printf("\n");

	/* step2: initialize animations */
	// create a new animation manager
	ChAnimationManager* animations = character->getAnimations();
	// load animations from this file

	// create a new animation, the name of the animation is necessary
	// get name of the animation from Fcollada
	// you can take the file name as animation name simply
	std::string animationName = filename;
	printf("animationName\n");
	printf("%s\n", animationName.c_str());
	ChAnimation* animation = animations->getAnimation(animationName);
	/* each bone of the skeleton has one corresponding ChTrack in one
	animation which records the movement of the bone */
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
		int frame_num = animationsBoneFrameNum[i];
//		printf("frame_num\n");
//		printf("%i\n", frame_num);
		// set key frame number
		track->init(frame_num);
		// for each key frame
		for(int j=0;j<frame_num;j++){
			// get relative change matrix of the bone in this key frame
			// from Fcollada
			Matrix matrix = animationsBoneFrameMatrix[i][j];
//			printf("boneMatrix\n");
//			printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
//			matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12], 
//			matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13], 
//			matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14], 
//			matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15]);
			// get this key frame's frame number in the whole animation
			// from Fcollada
			int frame_time = animationsBoneFrameTime[i][j];
//			printf("frame_time\n");
//			printf("%i\n", frame_time);
			// add the key frame into the track
			track->addKeyFrame(matrix,frame_time);
		}
//		printf("\n");
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
//		float* texCoord = character_vbo_texcoords[i];
		// get length of texCoord
//		int tcSize = character_vbo_size * 2;
		// get texture id
//		GLuint texId = character_vbo_texid[i];
		// set i-th vbo's texture
//		character->setVBOTexture(texCoord,tcSize,texId,i);
		// get material information of that vbo
//		float am[4] = {character_vbo_am[i][0], character_vbo_am[i][1], character_vbo_am[i][2], character_vbo_am[i][3]};
//		float di[4] = {character_vbo_di[i][0], character_vbo_di[i][1], character_vbo_di[i][2], character_vbo_di[i][3]};
//		float sp[4] = {character_vbo_sp[i][0], character_vbo_sp[i][1], character_vbo_sp[i][2], character_vbo_sp[i][3]};
//		float em[4] = {character_vbo_em[i][0], character_vbo_em[i][1], character_vbo_em[i][2], character_vbo_em[i][3]};
//		float sh = character_vbo_sh[i];
		// set i-th vbo's material
//		character->setVBOMaterial(am,di,sp,em,sh,i);
	}

	/* step6: add the character to the ObjectManager */
	objectManager->addVBOObject(character);

}

int * FColladaModelLoader::getSkinVertexIndex(int begin, int end){
	assert(begin != end);
	int * tempSkinVertexIndex = new int[end-begin];
	for(int i=0; i<end - begin; i++)
	{
		tempSkinVertexIndex[i] = begin + i;
	}
	return tempSkinVertexIndex;
}


int FColladaModelLoader::getBoneNumber()
{
	return boneNumber;
}
//store the vertices, normals, texturecoords and create staticobjects to display
void FColladaModelLoader::storeVertices(int kind)
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
				if(kind == 2)
				{
					//create the staticobject with vertexs
					StaticObject* objects = new StaticObject(m_vbo_vertices, m_num_vertices * 3);

					//set the normals of the staticobject
					objects->setNormals(m_vbo_normals, m_num_vertices * 3);

					//set the texcoords of the staticobject
					m_total_texcoords.push_back(m_vbo_texcoords);

					m_size_texcoords.push_back(m_num_vertices * 2);

					//add the objects to objectmanage
					objectManager->addVBOObject(objects);
				}
			}
			m_ptrs_geometries.push_back(mesh);
		}
	}
}





void FColladaModelLoader::storeLight(){
	// how many lights there are?
	FCDLightLibrary* lightlib=m_document->GetLightLibrary();
	m_num_lights=(int) lightlib->GetEntityCount();

	// copy lights to my structures
	FCDLight* light;

	for (int i=0; i<m_num_lights; i++) {
		light = lightlib->GetEntity(i);

		// ambiental light
		if (light->GetLightType()==FCDLight::AMBIENT) {
			// add our new light
			m_ptrs_Ambientlights.push_back(light);
			continue;
		}

		// spot light (luz focal)
		if (light->GetLightType()==FCDLight::SPOT) {
			// add our new light
			m_ptrs_Spotlights.push_back(light);
			continue;
		}

		// directional  light
		if (light->GetLightType()==FCDLight::DIRECTIONAL) {
			// add our new light
			m_ptrs_Directionallights.push_back(light);
			continue;
		}

		// point light
		if (light->GetLightType()==FCDLight::POINT) {
			// add our new light
			m_ptrs_Pointlights.push_back(light);
			continue;
		}
	}
}

//store all the textures that contains in the document
void FColladaModelLoader::storeTexture(){
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
void FColladaModelLoader::storeMaterials(){
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

void FColladaModelLoader::storeCamera(){
	// copy cameras to my structures
	// cameras
	FCDCameraLibrary* cameralib=m_document->GetCameraLibrary();
	m_num_cameras=(int) cameralib->GetEntityCount();

	FCDCamera* camera;
	for (int i=0; i<m_num_cameras; i++) {
		camera = cameralib->GetEntity(i);

		// perspective camera
		if (camera->IsPerspective()==true) {
			// add our new camera
			m_ptrs_Perspectivecameras.push_back(camera);
			continue;
		}

		// orthographic camera
		if (camera->IsOrthographic()==true) {
			// add our new camera
			m_ptrs_Orthographiccameras.push_back(camera);
			continue;
		}
	}

}

void FColladaModelLoader::storeAnimation(){
	// how many animations are there?
	FCDAnimationLibrary* animationlib=m_document->GetAnimationLibrary();
	m_num_animations=(int) animationlib->GetEntityCount();
	animationsBoneName = new std::string[m_num_animations];
	FCDAnimation* animation;
	for (int i=0; i<m_num_animations; i++) {
		animation = animationlib->GetEntity(i);
		m_ptrs_animation.push_back(animation);
	}
}

//build the scene include the material of the polygons, the texture of the polygons and the matrix of the bone.
void FColladaModelLoader::buildScene(FCDSceneNode* node_origin, int kind)
{
	//child scene node
	FCDSceneNode* child_origin;

	initBoneScene(node_origin);

	buildSceneInstance(node_origin, kind);

	buildSceneMatrix(node_origin);

	for (int i=0; i<(int)node_origin->GetChildrenCount(); i++) {
		child_origin=node_origin->GetChild(i);
		buildScene(child_origin, kind);
	}
}
void FColladaModelLoader::initBoneScene(FCDSceneNode* node_origin){
	if(node_origin ->GetJointFlag() == true){
		int index = getBoneIndexByName(node_origin->GetSubId().c_str());
		if(index >= 0 && index< boneNumber)
		{
			if(node_origin->GetParentCount() > 0){
				int parentIndex = getBoneIndexByName(node_origin->GetParent(0)->GetSubId().c_str());
				if(parentIndex >= 0 && parentIndex< boneNumber)
				{
					boneParentName[index] = boneName[parentIndex];
				}
				else{
					boneParentName[index] = "";
				}
			}
			else{
				boneParentName[index] = "";
			}

			int boneChildCount = 0;
			for(int i = 0; i< (int)node_origin->GetChildrenCount(); i++)
			{
				int tempChildIndex= getBoneIndexByName(node_origin->GetChild(i)->GetSubId().c_str());
				if(tempChildIndex >= 0 && tempChildIndex< boneNumber){
					if(node_origin->GetChild(i)->GetJointFlag() ==true)
					boneChildCount++;
				}
			}
			boneChildNum[index] = boneChildCount;
			int tempChildNum = boneChildNum[index];
			boneChildName[index] = new std::string[tempChildNum];
			int tempChildCount = 0;
			for(int i = 0; i< (int)node_origin->GetChildrenCount(); i++){
				if(node_origin->GetChild(i)->GetJointFlag() ==true){
					int tempChildIndex= getBoneIndexByName(node_origin->GetChild(i)->GetSubId().c_str());
					if(tempChildIndex >= 0 && tempChildIndex< boneNumber){
						boneChildName[index][tempChildCount] = boneName[tempChildIndex];
						tempChildCount++;
					}
				}
			}
		}
	}
}

int FColladaModelLoader::getBoneIndexByName(std::string name)
{
	for(int i=0; i<boneNumber; i++)
	{
		if(boneName[i]==name)
			return i;
	}
	return -1;
}

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
					charactervboNum = m_num_polygons;
					character_vbo_vertices = new float*[m_num_polygons];
					character_vbo_normals = new float*[m_num_polygons];
					character_vbo_texcoords = new float*[m_num_polygons];
					character_vbo_am = new float*[m_num_polygons];
					character_vbo_di = new float*[m_num_polygons];
					character_vbo_sp = new float*[m_num_polygons];
					character_vbo_em = new float*[m_num_polygons];
					character_vbo_sh = new float[m_num_polygons];
					character_vbo_texid = new GLuint[m_num_polygons];

					skinPolygonIndex = new int[m_num_polygons];
					int tempSkinPolygonIndexCount = 0;

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
						character_vbo_size = m_num_vertices;
						character_vbo_vertices[p] = m_vbo_vertices;
						character_vbo_normals[p] = m_vbo_normals;
						character_vbo_texcoords[p] = m_vbo_texcoords;
						tempPolygonVertexIndex += m_num_vertices;
						skinPolygonIndex[p] = tempSkinPolygonIndexCount;
						tempSkinPolygonIndexCount += m_num_vertices;
					}
					for (int i= 0; i<skinVertexNum; i++)
					{
						FCDSkinControllerVertex* influences = skin->GetVertexInfluence(skinPolygonVertexIndex[i]);
						skinVertexBoneCount[i] = (int)influences->GetPairCount();
						skinVertexBoneName[i] = new std::string[skinVertexBoneCount[i]];
						skinVertexBonePower[i] = new float[skinVertexBoneCount[i]];
						for(int j =0; j < (int)influences->GetPairCount(); j++)
						{
							FCDJointWeightPair* pairs = influences->GetPair(j);
							skinVertexBoneName[i][j] = boneName[(int)pairs->jointIndex];
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

void FColladaModelLoader::buildSkin(FCDSkinController* skin){
	boneNumber = (int)skin->GetJointCount();
	boneName = new std::string[boneNumber];
	boneMatrix = new Matrix[boneNumber];
	boneParentName = new std::string[boneNumber];
	boneChildNum = new int[boneNumber];
	boneChildName = new std::string*[boneNumber];
	animationsBoneFrameNum = new int[boneNumber];
	animationsBoneFrameMatrix = new Matrix*[boneNumber];
	animationsBoneFrameTime = new int*[boneNumber];

	
	for(int i = 0; i < (int)skin->GetJointCount(); i++)
	{
		FCDSkinControllerJoint* joint = skin->GetJoint(i);
		boneName[i] = joint->GetId().c_str();
		/*
		FMMatrix44 inverse = joint->GetBindPoseInverse() * skin->GetBindShapeTransform();
		float tempMatrixElement[] ={inverse.m[0][0], inverse.m[0][1], inverse.m[0][2], inverse.m[0][3], 
		inverse.m[1][0], inverse.m[1][1], inverse.m[1][2], inverse.m[1][3],
		inverse.m[2][0], inverse.m[2][1], inverse.m[2][2], inverse.m[2][3],
		inverse.m[3][0], inverse.m[3][1], inverse.m[3][2], inverse.m[3][3],};
		Matrix tempMatrix = Matrix(tempMatrixElement);
		boneMatrix[i] = tempMatrix;
		*/
	}

	for(int i = 0; i < (int)skin->GetInfluenceCount(); i++)
	{
		FCDSkinControllerVertex* influences = skin->GetVertexInfluence(i);
		for(int j =0; j < (int)influences->GetPairCount(); j++)
		{
			FCDJointWeightPair* pairs = influences->GetPair(j);
		}
	}
	
}
void FColladaModelLoader::buildSceneMatrix(FCDSceneNode* node_origin)
{
	if(node_origin ->GetJointFlag() == true){
		int index = getBoneIndexByName(node_origin->GetSubId().c_str());
		animationsBoneFrameNum[index] = 0;
		if(index >= 0 && index< boneNumber)
		{
			FMMatrix44 inverse = node_origin->ToMatrix();
			Matrix tempMatrix;
			for(int i=0;i<16;i++){
				tempMatrix[i]=inverse.m[i%4][i/4];
			}
			boneMatrix[index] = tempMatrix;

			FCDTransform*  trans_origin;

			// copy node transformations
			for (int i=0; i<(int)node_origin->GetTransformCount(); i++) {
				trans_origin=node_origin->GetTransform(i);

			// rotation
				if (trans_origin->GetType()== FCDTransform::ROTATION) {
					FCDTRotation* trans_rot=dynamic_cast<FCDTRotation*>(trans_origin); // casting
					/*
					// initialize curves
					FCDAnimationCurve* m_x_curve=NULL;
					FCDAnimationCurve* m_y_curve=NULL;
					FCDAnimationCurve* m_z_curve=NULL;
					FCDAnimationCurve* m_angle_curve=NULL;

					// is there any animation ?
					if (trans_rot->IsAnimated()) {
		
						FCDAnimationCurve* curve;

						// look for x animation
						curve=trans_rot->GetAnimated()->FindCurve(".X");
						if (curve!=NULL) {
							curve->GetKeyCount();
						}

						// look for y animation
						curve=trans_rot->GetAnimated()->FindCurve(".Y");
						if (curve!=NULL){

						}
		
						// look for z animation
						curve=trans_rot->GetAnimated()->FindCurve(".Z");
						if (curve!=NULL){	

						}

						// look for angle animation
						curve=trans_rot->GetAnimated()->FindCurve(".ANGLE");
						if (curve!=NULL){
	
						}
					}
					continue;
					*/
				}

				// translation
				if (trans_origin->GetType()== FCDTransform::TRANSLATION) {
					FCDTTranslation* trans_trans=dynamic_cast<FCDTTranslation*>(trans_origin); // casting
					/*
					// initialize curves
					FCDAnimationCurve* m_x_curve=NULL;
					FCDAnimationCurve* m_y_curve=NULL;
					FCDAnimationCurve* m_z_curve=NULL;
	
					// is there any animation ?
					if (trans_trans->IsAnimated()) {
	
						FCDAnimationCurve* curve;

						// look for x animation
						curve=trans_trans->GetAnimated()->FindCurve(".X");
						if (curve!=NULL){
	
						}

						// look for y animation
						curve=trans_trans->GetAnimated()->FindCurve(".Y");
						if (curve!=NULL) {
	
						}
	
						// look for z animation
						curve=trans_trans->GetAnimated()->FindCurve(".Z");
						if (curve!=NULL) {
	
						}
					}	
					continue;
					*/
				}

				// scale
				if (trans_origin->GetType()== FCDTransform::SCALE) {
					FCDTScale* trans_scale=dynamic_cast<FCDTScale*>(trans_origin); // casting 
					/*
					// initialize curves
					FCDAnimationCurve* m_x_curve=NULL;
					FCDAnimationCurve* m_y_curve=NULL;
					FCDAnimationCurve* m_z_curve=NULL;

					// is there any animation ?
					if (trans_scale->IsAnimated()) {
	
						FCDAnimationCurve* curve;
	
						// look for x animation
						curve=trans_scale->GetAnimated()->FindCurve(".X");
						if (curve!=NULL) {
	
						}
	
						// look for y animation
						curve=trans_scale->GetAnimated()->FindCurve(".Y");
						if (curve!=NULL) {
		
						}
	
						// look for z animation
						curve=trans_scale->GetAnimated()->FindCurve(".Z");
						if (curve!=NULL) {
		
						}
					}	
					continue; // actually, not necessary
					*/
				}

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
						animationsBoneFrameNum[index] = animationKeyCount;
						animationsBoneFrameMatrix[index] = new Matrix[animationKeyCount];
						animationsBoneFrameTime[index] = new int[animationKeyCount];
	
						for(int j = 0; j < animationKeyCount; j++)
						{
							animationsBoneFrameTime[index][j] = (int)((animated->GetCurves().at(0)[0]->GetKey(j)->input * 15 * 10) + 0.5);	
							float* tempAnimationMatrixFloat = new float[(int)animated->GetValueCount()];						
							for(int p = 0; p < (int)animated->GetValueCount(); p++)
							{
								tempAnimationMatrixFloat[p] = animated->GetCurves().at(p)[0]->GetKey(j)->output;
							}
							
							Matrix tempAnimationMatrix = Matrix(tempAnimationMatrixFloat);
							animationsBoneFrameMatrix[index][j] = tempAnimationMatrix;
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

//set the target material to the polygon
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
		character_vbo_am[polygonIndex] = new float[4];
		character_vbo_di[polygonIndex] = new float[4];
		character_vbo_sp[polygonIndex] = new float[4];
		character_vbo_em[polygonIndex] = new float[4];
		character_vbo_texid[polygonIndex] = 0;
		//if has texture, set the texture
		character_vbo_am[polygonIndex] = am;
		character_vbo_di[polygonIndex] = di;
		character_vbo_sp[polygonIndex] = sp;
		character_vbo_em[polygonIndex] = em;
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
			character_vbo_texid[polygonIndex] = tex;
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





void FColladaModelLoader::drawLine(FCDSceneNode* node_origin, int kind)
{
	FMVector3 v_origin;
	if(node_origin ->GetJointFlag() == true){
		v_origin = node_origin->CalculateWorldTransform().GetTranslation();
	}
	for (int i=0; i<(int)node_origin->GetChildrenCount(); i++) {
		FCDSceneNode* child_origin=node_origin->GetChild(i);
		if ( child_origin ->GetJointFlag() == true)
		{
			FMVector3 v_child;
			v_child = child_origin->CalculateWorldTransform().GetTranslation();
			SimpleLine *simpleLine= new SimpleLine();
			float dot1[3];
			dot1[0] = v_origin.x; dot1[1] = v_origin.y; dot1[2] = v_origin.z;
			float dot2[3];
			dot2[0] = v_child.x; dot2[1] = v_child.y; dot2[2] = v_child.z;
			simpleLine->setDot1(dot1);
			simpleLine->setDot2(dot2);
			simpleLines.push_back(simpleLine);
		}
		drawLine(child_origin, kind);
	}
}