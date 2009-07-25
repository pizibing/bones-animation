#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChBone.h"

// constructor
// @param name the name for the bone
ChBone::ChBone(const std::string &name)
: m_name(name),m_parentbone(NULL),m_childbones(NULL)
{
}

// destructor
ChBone::~ChBone(void){
	if(m_childbones)
		delete [] m_childbones;
	m_childbones = NULL;
}

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

// @return the id of parent bone
//			-1 if no parent bone
int ChBone::getParentId() const{
	if(m_parentbone){
		return m_parentbone->getId();
	}
	return -1;
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
	if(m_current_child_num>m_child_num){
		// there is no more room for new child bone
		return false;
	}
	m_childbones[m_current_child_num] = bone;
	m_current_child_num++;
	return false;
}

// get the original relative transform matrix
//const Matrix& ChBone::getOriginalTransformMatrix()const{
//	return m_originalTransformMatrix;
//}

// get the rotation transform in local space
const Quaternion& ChBone::getRotation() const{
	return m_rotation;
}

// get the translation transform in local space
const Vector3D& ChBone::getTranslation() const{
	return m_translation;
}

// get the rotation transform in world space
const Quaternion& ChBone::getAbsoluteRotation() const{
	return m_absoluteRotation;
}

// get the translation transform in world space
const Vector3D& ChBone::getAbsoluteTranslation() const{
	return m_absoluteTranslation;
}

// set the original relative transform matrix
void ChBone::setTransformMatrix(const Matrix &m){
	m_rotation = m.getRotation();
	m_translation = m.getTranslation();
}

// set the absolute transform matrix
void ChBone::setAbsolteTransformMatrix(const Matrix &m){
	m_absoluteRotation = m.getRotation();
	m_translation = m.getTranslation();
	m_bindPoseInverse = m.getInverseMatrix();
}

// get the bind pose inverse matrix in world space
const Matrix & ChBone::getBindPoseInverse(){
	return m_bindPoseInverse;
}
// set the bind pose inverse matrix
void ChBone::setBindPoseInverse(const Matrix & m){
	m_bindPoseInverse = m;
}

// calculate transform in world space and for all child bones
void ChBone::calculateAbsoluteTransform(){
	if(m_parentbone){
		m_absoluteTranslation = m_parentbone->m_absoluteRotation * m_translation
									+ m_parentbone->m_absoluteTranslation;
		m_absoluteRotation = m_parentbone->m_absoluteRotation * m_rotation;
	}
	else {
		// this is a root bone
		m_absoluteRotation = m_rotation;
		m_absoluteTranslation = m_translation;
	}
	//set bind pose inverse matrix
	Matrix m;
	m.set(m_absoluteRotation,m_absoluteTranslation);
	m_bindPoseInverse = m.getInverseMatrix();

	// calculate child bones
	for(int i=0;i<m_child_num;i++){
		m_childbones[i]->calculateAbsoluteTransform();
	}
}

// initialize this bone with child_num child bones
// @param child_num child bone number
// @return true if successful
//		   false if reinitialized or no more memory
bool ChBone::init(int child_num){
	if(m_childbones)return false;

	m_childbones = new ChBone*[child_num];
	if(!m_childbones) return false;
	for(int i=0;i<child_num;i++)
		m_childbones[i] = NULL;
	m_child_num = child_num;
	m_current_child_num = 0;
	return true;
}

