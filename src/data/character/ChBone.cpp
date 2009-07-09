#include "../matrixlib/matrix.h"
#include "ChBone.h"

ChBone::ChBone(const std::string &name){
	m_name = name;
}

ChBone::~ChBone(void){}

const std::string& ChBone::getName()const{
	return m_name;
}

ChBone * ChBone::getParentBone()const{
	return m_parentbone;
}
void ChBone::SetParentBone(ChBone * parent){
	m_parentbone = parent;
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

void ChBone::setOriginalTransformMatrix(const Matrix &m){
	m_originalTransformMatrix = m;
}


// initialize this bone with child_num child bones
// @param child_num child bone number
// @return true if successful
bool ChBone::init(int child_num){
	return false;
}
