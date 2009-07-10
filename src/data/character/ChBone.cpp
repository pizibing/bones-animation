#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
#include "ChBone.h"

// constructor
// @param name the name for the bone
ChBone::ChBone(const std::string &name){
	m_name = name;
}

// destructor
ChBone::~ChBone(void){}

// the name for the bone
const std::string& ChBone::getName()const{
	return m_name;
}

// @return the index in skeleton
int ChBone::getId() const{
	return m_id;
}
// set the index in skeleton
void ChBone::setId(int id){
	m_id = id;
}

// @return the pointer of the parent bone
ChBone * ChBone::getParentBone()const{
	return m_parentbone;
}

// set the parent bone
void ChBone::SetParentBone(ChBone * parent){
	m_parentbone = parent;
}

// get the array of child bone pointers
ChBone** ChBone::getAllChildBones()const{
	return m_childbones;
}

// @return the number of child bones
int ChBone::getChildBoneNum(){
	return m_child_num;
}

// add a child bone 
// @return false if no room for new child bone
bool ChBone::addChildBone(ChBone *bone){
	return false;
}

// get the original relative transform matrix
const Matrix& ChBone::getOriginalTransformMatrix()const{
	return m_originalTransformMatrix;
}

// set the original relative transform matrix
void ChBone::setOriginalTransformMatrix(const Matrix &m){
	m_originalTransformMatrix = m;
}


// initialize this bone with child_num child bones
// @param child_num child bone number
// @return true if successful
bool ChBone::init(int child_num){
	return false;
}

