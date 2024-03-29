#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChBone.h"
#include "ChSkeleton.h"
#include <assert.h>


// constructor
ChSkeleton::ChSkeleton(void)
: m_bones(NULL),m_rootBone(NULL),m_bone_num(0)
{
}

// destructor
ChSkeleton::~ChSkeleton(void){
	for(int i=0;i<m_bone_num;i++){
		delete m_bones[i];
	}
	if(m_bones)
		delete []m_bones;
	m_bones = NULL;
}

// get bone by name
// @param name the name for the bone
// @return the pointer for the bone with the name
//         if no bone has the name, create a new bone
ChBone* ChSkeleton::getBone(const std::string &name){
	std::map<std::string,int>::iterator i = m_boneMap.find(name);
	if(i!=m_boneMap.end()){
		return m_bones[i->second];
	}
	int id = m_boneMap.size();
	if(id<m_bone_num){
		m_bones[id] = new ChBone(name);
		m_bones[id]->setId(id);
		m_boneMap[name] = id;
		return m_bones[id];
	}
	return NULL;
}

// get bone by index
// @param boneId the index for the bone
// @return the pointer for the bone with the index
//         if no bone has the index return NULL
ChBone* ChSkeleton::getBone(int boneId){
	if(boneId>=0&&boneId<m_bone_num){
		return m_bones[boneId];
	}
	return NULL;
}

// get the index of bone with the name
// @param name the name for the bone
// @return the index of the bone 
//			   -1 if cannot find
int ChSkeleton::getBoneId(const std::string &name){
	std::map<std::string,int>::iterator i = m_boneMap.find(name);
	if(i!=m_boneMap.end())return i->second;
	return -1; 
}

ChBone** ChSkeleton::getAllBones()const{
	return m_bones;
}

int ChSkeleton::getBoneNum()const{
	return m_bone_num;
}

// @return the root bone pointer
ChBone* ChSkeleton::getRootBone()const{
	assert(m_rootBone);
	return m_rootBone;
}

// get the array of all bones
void ChSkeleton::setRootBone(ChBone *bone){
	m_rootBone = bone;
}

// calculate transform in world space
void ChSkeleton::calculateAbsoluteTransform(){
	assert(m_rootBone);
	m_rootBone->calculateAbsoluteTransform();
}

// init the pointer array for bones
// skeleton can be initialized only once
// @param bone_num the number of bones
// @return true if successful 
bool ChSkeleton::init(int bone_num){
	if(m_bones)return false;

	m_bones = new ChBone*[bone_num];
	if(!m_bones)return false;
	for(int i=0;i<bone_num;i++){
		m_bones[i] = NULL;
	}
	m_bone_num = bone_num;
	return false;
}
