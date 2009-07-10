#include "../matrixlib/mtxlib.h"
//#include "../matrixlib/matrix.h"
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

