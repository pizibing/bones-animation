#include "ModelLoader.h"
#include "SimpleModelLoader.h"
#include "../matrixlib/mtxlib.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../data/MoveSelfObject.h"
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
		bone->setOriginalTransformMatrix(matrix);
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
	// get root bone's name from Fcollada
	std::string rootName = "";
	// get root's pointer
	ChBone* root = skeleton->getBone(rootName);
	// set root
	skeleton->setRootBone(root);

	/* step2: initialize animations */
	// create a new animation manager
	ChAnimationManager* animations = character->getAnimations();
	// get animation number of this CharacterObject from Fcollada
	int animation_num = 0;
	// set animation number
	animations->init(animation_num);
	// for each animation
	for(int i = 0; i < animation_num; i++){
		// create a new animation, the name of the animation is necessary
		// get name of the animation from Fcollada
		std::string animationName = "";
		ChAnimation* animation = animations->getAnimation(animationName);
		/* each bone of the skeleton has one corresponding ChTrack in one
		   animation which records the movement of the bone */
		// for each bone (how to go through all the bones is your choice)
		for(;;){
			// get bone's name
			std::string boneName = "";
			// get bone's relative ChTrack
			ChTrack* track = animation->getTrack(boneName);
			/* initialize the track */
			// get key frame number of this track from Fcollada
			int frame_num = 0;
			// set key frame number
			track->init(frame_num);
			// for each key frame
			for(;;){
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
	}

	return true;
}
