#include "ChVertexInstance.h"

ChVertexInstance::ChVertexInstance(int id){
	this->id = id;
	hasNormal = false;
}

ChVertexInstance::~ChVertexInstance(void){
}

// get function of id
int ChVertexInstance::getId(){
	return id;
}

// set function of position, p should be an array of 3
void ChVertexInstance::setPosition(float* p){
	for(int i = 0; i < 3; i++)
		position[i] = p[i];
}

// get function of position
float* ChVertexInstance::getPosition(){
	return position;
}

// set function of normal, n should be an array of 3
void ChVertexInstance::setNormal(float* n){
	for(int i = 0; i < 3; i++)
		normal[i] = n[i];
}

// get function of normal
float* ChVertexInstance::getNormal(){
	return normal;
}

// set function of hasNormal
void ChVertexInstance::setHasNormal(bool hn){
	hasNormal = hn;
}

// get function of hasNormal
bool ChVertexInstance::getHasNormal(){
	return hasNormal;
}