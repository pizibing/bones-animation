#include "global.h"
#include "coreskeleton.h"
#include "corebone.h"

CoreSkeleton::CoreSkeleton(){}

CoreSkeleton::~CoreSkeleton(){
	std::vector<CoreBone*>::iterator coreboneIterator;
	for(coreboneIterator = m_coreBones.begin(); coreboneIterator != m_coreBones.end(); ++coreboneIterator){
		delete (*coreboneIterator);
	}
}

/*
* create a new CoreBone,add it to the bone vector
* @param name the name for the bone
* @param root whether this is a root bone
* @return the pointer to the new CoreBone
*		 0 if failed to create a new CoreBone
*/
CoreBone * CoreSkeleton::newCoreBone(const std::string name, bool root){
	CoreBone *corebone = new CoreBone(name);
	if(corebone!=NULL){
		int id = m_coreBones.size();
		m_coreBoneNamesMap[name] = id;
		corebone->setCoreSkeleton(this);
		if(root) m_rootCoreBones.push_back(corebone);
		this->m_coreBones.push_back(corebone);
	}
	return NULL;
}

/**
* Calculates the current state.
*
* This function calculates the current state (absolute translation and
* rotation, as well as the bone space transformation) of the bone instance
* and all its children.
**/
void CoreSkeleton::calculateState(){
	std::vector<CoreBone *>::iterator rootCoreBoneIterator;
	for(rootCoreBoneIterator = m_rootCoreBones.begin(); rootCoreBoneIterator != m_rootCoreBones.end(); ++rootCoreBoneIterator)
	{
		(*rootCoreBoneIterator)->calculateState();
	}
}

CoreBone* CoreSkeleton::getCoreBone(int coreBoneId){
	return m_coreBones[coreBoneId];
}
CoreBone* CoreSkeleton::getCoreBone(const std::string& name){
	return m_coreBones[m_coreBoneNamesMap[name]];
}

int CoreSkeleton::getCoreBoneId(const std::string& name){
	return m_coreBoneNamesMap[name];
}

std::vector<CoreBone *>& CoreSkeleton::getRootCoreBones(){
	return m_rootCoreBones;
}
std::vector<CoreBone *>& CoreSkeleton::getCoreBones(){
	return m_coreBones;
}

void CoreSkeleton::scale(float factor){
	std::vector<CoreBone *>::iterator rootCoreBoneIterator;
	for(rootCoreBoneIterator = m_rootCoreBones.begin(); rootCoreBoneIterator != m_rootCoreBones.end(); ++rootCoreBoneIterator)
	{
		(*rootCoreBoneIterator)->scale(factor);
	}
}


