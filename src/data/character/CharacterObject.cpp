#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "CharacterObject.h"

CharacterObject::CharacterObject(void){
}

CharacterObject::~CharacterObject(void){
}

// return a list of VBOMeshes that this VBOObject represent
// this function return a pointer to the first VBOMesh of the 
// list and num will be changed to the size of the list
VBOMesh* CharacterObject::representInVBOMesh(int* num){
	*num = meshSize;
	return vbomeshes;
}

//return the type of this object
int CharacterObject::getType(){
	return type;
}

//return the id of this object
int CharacterObject::getID(){
	return id;
}
