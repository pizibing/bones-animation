#include "ModelLoader.h"
#include "SimpleModelLoader.h"
#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../data/MoveSelfObject.h"
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

SimpleModelLoader::SimpleModelLoader(void){
}

SimpleModelLoader::~SimpleModelLoader(void){
}

bool SimpleModelLoader::loadModel(int kind, char* path){
	objectManager = ObjectManager::getInstance();

	/** following is the demo initial of CharacterObject **/

	// create a new CharacterObject
	CharacterObject* character = new CharacterObject();

	/* step1: initialize skeleton */
	// create a new ChSkeleton
	ChSkeleton* skeleton = character->getSkeleton();
	// get bone number of the skeleton from Fcollada
	int bone_num = 0;
	// set bone number to skeleton
	skeleton->init(bone_num);
	// for each bone
	for(int i = 0; i < bone_num; i++){
		/* create a new ChBone, the name of the bone is necessary */
		// get bone name from Fcollada
		std::string name = "";
		ChBone* bone = skeleton->getBone(name);
		/* initialize the bone */
		// get relative transform matrix of the bone from Fcollada
		Matrix matrix = Matrix();
		// set bone's relative transform matrix 
		bone->setTransformMatrix(matrix);
		// get parent's name
		std::string parentName = "";
		// get parent's pointer
		ChBone* parent = skeleton->getBone(parentName);
		// set parent
		bone->SetParentBone(parent);
		// get child number from Fcollada
		int child_num = 0;
		// set child number
		bone->init(child_num);
		// for each child
		for(int j = 0; j < child_num; j++){
			// get child's name from Fcollada
			std::string childName = "";
			// get child's pointer
			ChBone* child = skeleton->getBone(childName);
			// set child
			bone->addChildBone(child);
		}
	}
	// calculate tranform in world space
	skeleton->calculateAbsoluteTransform();

	// get root bone's name from Fcollada
	std::string rootName = "";
	// get root's pointer
	ChBone* root = skeleton->getBone(rootName);
	// set root
	skeleton->setRootBone(root);

	/* step2: initialize animations */
	// create a new animation manager
	ChAnimationManager* animations = character->getAnimations();
	// load animations from this file
	
	// create a new animation, the name of the animation is necessary
	// get name of the animation from Fcollada
	// you can take the file name as animation name simply
	std::string animationName = "filename";
	ChAnimation* animation = animations->getAnimation(animationName);
	/* each bone of the skeleton has one corresponding ChTrack in one
	   animation which records the movement of the bone */
	// for each bone (how to go through all the bones is your choice)
	for(int i=0;i<skeleton->getBoneNum();i++){
		// get bone's name
		std::string boneName = "";
		// get bone's relative ChTrack
		ChTrack* track = animation->getTrack(boneName);
		/* initialize the track */
		// get key frame number of this track from Fcollada
		// usually is the animation total key frame number
		int frame_num = 0;
		// set key frame number
		track->init(frame_num);
		// for each key frame
		for(int j=0;j<frame_num;j++){
			// get relative change matrix of the bone in this key frame
			// from Fcollada
			Matrix matrix = Matrix();
			// get this key frame's frame number in the whole animation
			// from Fcollada
			int frame_time = 0;
			// add the key frame into the track
			track->addKeyFrame(matrix,frame_time);
		}
	}

	/* step3: initialize skin */
	// create new ChSkin
	ChSkin* skin = character->getSkin();
	// get vertex number of the skin from Fcollada
	int vertexNum = 0;
	// set vertex number(no duplicated vertex is allowed)
	skin->initVertices(vertexNum);
	// initialize each vertex in the skin
	for(int i = 0; i < vertexNum; i++){
		// create a vertex
		ChVertex* vertex = skin->initVertice(vertexNum);
		// get position of the vertex to be added from Fcollada
		float px,py,pz;
		// create a new vertex
		vertex->setDefaultPosition(px,py,pz);
		// get normal of the vertex from Fcollada
		float nx,ny,nz;
		// set normal
		vertex->setDefaultNormal(nx,ny,nz);
		// get number of bones this vertex relates to from Fcollada
		int relatedBoneNum = 0;
		// set related bone number
		vertex->initPairs(relatedBoneNum);
		/* pair is a struct that stores the related bones of the
		  vertex and their power */
		// for each pair
		for(int j = 0; j < relatedBoneNum; j++){
			// get related bone name from Fcollada
			char* boneName = "";
			// get power of the bone to this vertex
			// please notice that the sum of all the powers of a vertex
			// should be 1
			float power = 0;
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
	int vboNum = 0;
	// set vbo number
	character->initVBOs(vboNum);
	// for each vbo
	for(int i = 0; i < vboNum; i++){
		/* vertices is an array of ids of ChVertex that is contained 
		   in this VBO, you can get the id of each vertex use getId()
		   function of ChVertex, perhaps you can generate all vertices'
		   id information when you built the skin */
		// generate each vbo's vertices
		int* vertices = 0;
		// get the length of vertices
		int vSize = 0;
		// initialize i-th vbo
		character->initVBO(vertices,vSize,i);
		// get texture coordinates of vertices
		float* texCoord = 0;
		// get length of texCoord
		int tcSize = 0;
		// get texture id
		int texId = 0;
		// set i-th vbo's texture
		character->setVBOTexture(texCoord,tcSize,texId,i);
		// get material information of that vbo
		GLfloat am[4];
		GLfloat di[4];
		GLfloat sp[4];
		GLfloat em[4];
		GLfloat sh;
		// set i-th vbo's material
		character->setVBOMaterial(am,di,sp,em,sh,i);
	}

	/* step6: add the character to the ObjectManager */
	objectManager->addVBOObject(character);

	return true;
}
