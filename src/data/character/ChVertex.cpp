#include <assert.h>
#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChVertex.h"
#include "../data/character/ChBone.h"
#include "../data/character/ChSkeleton.h"

// constructor
ChVertex::ChVertex(ChSkeleton* skeleton){
	// init normal
	hasNormal = false;
	// init pairNum
	pairNum = 0;
	// init skeleton
	this->skeleton = skeleton;

	pairs = 0;
}

// destructor
ChVertex::~ChVertex(void){
	if(pairs) delete[] pairs;
	pairs = 0;
}

// set the default position of the vertex
void ChVertex::setDefaultPosition(float px, float py, float pz){
	// init position
	position[0] = px;
	position[1] = py;
	position[2] = pz;
}

// set the default normal of the vertex
// hasNormal will be set to true and normal[3] will be set
// to the given value
void ChVertex::setDefaultNormal(float nx, float ny, float nz){
	hasNormal = true;
	normal[0] = nx;
	normal[1] = ny;
	normal[2] = nz;
}

// initialize the pairs 
// if pairs has never been initialized, a new array will be 
// created whose length will be num
// else nothing happens
void ChVertex::initPairs(int num){
	// num should be bigger than 0
	assert(num > 0);

	// if pairs has been initialized
	if(pairs) return;
	else{
		pairs = new VBpair[num];
		pairNum = num;
	}
}

// initialize the pairNum-th VBpair of pairs
// matrix should be a float[16]
// boneId, matrix, power will be set to the corresponding properties
// in VBpair
void ChVertex::initPair(int pairNum, char* boneName, float power){
	// pairNum should be smaller than pairNum
	assert(pairNum < this->pairNum);

	// init pair
	pairs[pairNum].boneId = skeleton->getBone(boneName)->getId();
	pairs[pairNum].power = power;
}

// set id of this vertex to id
void ChVertex::setId(int id){
	this->id = id;
}

// get the id of this vertex, if id has not been set , the default 
// id is -1
int ChVertex::getId(){
	return id;
}

// get function of position
float* ChVertex::getPosition(){
	return position;
}

// get function of normal
float* ChVertex::getNormal(){
	return normal;
}

// get function of hasNormal
bool ChVertex::getHasNormal(){
	return hasNormal;
}

// get function of pairs
VBpair* ChVertex::getPairs(){
	return pairs;
}

// get function of pairNum
int ChVertex::getPairNum(){
	return pairNum;
}