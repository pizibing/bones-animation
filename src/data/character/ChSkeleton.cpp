#include "ChSkeleton.h"
#include "ChBone.h"

// constructor
ChSkeleton::ChSkeleton(void){
}

// destructor
ChSkeleton::~ChSkeleton(void){
}

// calculate absolute transform for each bone
void ChSkeleton::calculateState(){
}

// get bone by name
// @param name the name for the bone
// @return the pointer for the bone with the name
//         if no bone has the name, create a new bone
ChBone* ChSkeleton::getBone(const std::string &name){
	return NULL;
}

ChBone* ChSkeleton::getRootBone(){
	return m_rootBone;
}

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
