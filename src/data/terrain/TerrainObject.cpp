#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "TerrainObject.h"
#include "../managers/ObjectManager.h"

// constructor
TerrainObject::TerrainObject(void){
	// StaticObject is static and will never be changed,
	// thus initialize vbomesh using GL_STATIC_DRAW_ARB 
	//vbomesh = new VBOMesh(vertex,size,GL_STATIC_DRAW_ARB);

	// type is OBJECT_TYPE_STATIC
	type = OBJECT_TYPE_STATIC;

	// generate id
	ObjectManager* om = ObjectManager::getInstance();
	id = om->generateID();
}

// destructor
TerrainObject::~TerrainObject(void){
}

// return a list of VBOMeshes that this VBOObject represent
// this function return a pointer to the first VBOMesh of the 
// list and num will be changed to the size of the list
VBOMesh* TerrainObject::representInVBOMesh(int* num){
	// the number of vbomeshes that represent terrain is 1
	*num = 1;

	return vbomesh;
}

//return the type of this object
int TerrainObject::getType(){
	return type;
}

//return the id of this object
int TerrainObject::getID(){
	return id;
}
