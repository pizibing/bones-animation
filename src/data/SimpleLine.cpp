#include <assert.h>
#include "SimpleLine.h"

// constructor
SimpleLine::SimpleLine(void){
}

// destructor
SimpleLine::~SimpleLine(void){
}

// set function of dot1
void SimpleLine::setDot1(const float* dot1){
	// dot1 should not be null
	assert(dot1);

	// set x,y,z
	for(int i = 0; i < 3; i++)
		this->dot1[i] = dot1[i];
}

// set function of dot2
void SimpleLine::setDot2(const float* dot2){
	// dot2 should not be null
	assert(dot2);

	// set x,y,z
	for(int i = 0; i < 3; i++)
		this->dot2[i] = dot2[i];
}

// get function of dot1
const float* SimpleLine::getDot1(){
	return dot1;
}

// get function of dot2
const float* SimpleLine::getDot2(){
	return dot2;
}

