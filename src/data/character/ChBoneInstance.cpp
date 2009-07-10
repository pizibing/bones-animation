#include "ChBoneInstance.h"
#include "../matrixlib/mtxlib.h"

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
	if(matrix) delete matrix;
}

// set function of matrix
void ChBoneInstance::setMatrix(matrix44* matrix){
	// matrix should not be null
	assert(matrix);

	this->matrix = matrix;
}

// get function of matrix
matrix44* ChBoneInstance::getMatrix(){
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
