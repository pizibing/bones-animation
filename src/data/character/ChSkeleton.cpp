#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
#include "ChSkeleton.h"
#include "ChBone.h"

// constructor
ChSkeleton::ChSkeleton(void){
}

// destructor
ChSkeleton::~ChSkeleton(void){
}

// get bone by name
// @param name the name for the bone
// @return the pointer for the bone with the name
//         if no bone has the name, create a new bone
ChBone* ChSkeleton::getBone(const std::string &name)const{
	return NULL;
}

// get bone by index
// @param boneId the index for the bone
// @return the pointer for the bone with the index
//         if no bone has the index return NULL
ChBone* ChSkeleton::getBone(int boneId){
	return NULL;
}

// get the index of bone with the name
// @param name the name for the bone
// @return the index of the bone 
int ChSkeleton::getBoneId(const std::string &name){
	return m_boneMap[name];
}

ChBone** ChSkeleton::getAllBones()const{
	return m_bones;
}

int ChSkeleton::getBoneNum()const{
	return m_bone_num;
}

// @return the root bone pointer
ChBone* ChSkeleton::getRootBone()const{
	return m_rootBone;
}

// get the array of all bones
void ChSkeleton::setRootBone(ChBone *bone){
	m_rootBone = bone;
}

// init the pointer array for bones
// skeleton can be initialized only once
// @param bone_num the number of bones
// @return true if successful 
bool ChSkeleton::init(int bone_num){
	return false;
}
