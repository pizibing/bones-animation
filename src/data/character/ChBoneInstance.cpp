#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChBoneInstance.h"


// constructor
// initialize id to id
ChBoneInstance::ChBoneInstance(int id){
	this->id = id;
	this->fatherId = -1;
	this->childCount = 0;
}

//destructor
ChBoneInstance::~ChBoneInstance(void){
	if(childId) delete[] childId;
}

// set function of rotation
void ChBoneInstance::setRotation(const Quaternion &q){
	rotation = q;
}

// get function of rotation
const Quaternion & ChBoneInstance::getRotation(){
	return rotation;
}

// set function of translation
void ChBoneInstance::setTranslation(const Vector3D &v){
	translation = v;
}

// get function of translation
const Vector3D & ChBoneInstance::getTranslation(){
	return translation;
}

// set function of absolute rotation
void ChBoneInstance::setAbsoluteRotation(const Quaternion &q){
	absoluteRotation = q;
}

// get function of absolute rotation
const Quaternion & ChBoneInstance::getAbsoluteRotation(){
	return absoluteRotation;
}

// set function of absolute translation
void ChBoneInstance::setAbsoluteTranslation(const Vector3D &v){
	absoluteTranslation = v;
}

// get function of absolute translation
const Vector3D & ChBoneInstance::getAbsoluteTranslation(){
	return absoluteTranslation;
}

// set function of matrix
void ChBoneInstance::setMatrix(const Matrix &matrix){
	this->matrix = matrix;
}

// get function of matrix
const Matrix & ChBoneInstance::getMatrix(){
	return matrix;
}

// get function of id
int ChBoneInstance::getId(){
	return id;
}

// set function of fatherId
void ChBoneInstance::setFatherId(int id){
	this->fatherId = id;
}

// get function of fatherId ,root's fatherId is -1
int ChBoneInstance::getFatherId(){
	return fatherId;
}

// initialize the childId array, set the length to num
// if childId has been initialized, do nothing
void ChBoneInstance::initChildId(int num){
	// if childId has been initialized
	if(childId) return;
	else{
		childId = new int[num];
		childCount = num;
	}
}

// set the num-th childId to id
void ChBoneInstance::setChildId(int num, int id){
	childId[num] = id;
}

// get function of childId
// return the pointer to the first element of childId, num will 
// be set to the length of childId
// if this bone instance is the leaf, NULL will be return, num
// will be set to 0
int* ChBoneInstance::getChildId(int* num){
	// set num to chlidCount
	*num = childCount;

	return childId;
}
