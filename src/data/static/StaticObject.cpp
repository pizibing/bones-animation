#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "StaticObject.h"
#include "../managers/ObjectManager.h"

//constructor
StaticObject::StaticObject(const GLfloat* vertex, int size){
	// StaticObject is static and will never be changed,
	// thus initialize vbomesh using GL_STATIC_DRAW_ARB 
	vbomesh = new VBOMesh(vertex,size,GL_STATIC_DRAW_ARB);
	
	// type is OBJECT_TYPE_STATIC
	type = OBJECT_TYPE_STATIC;

	// generate id
	ObjectManager* om = ObjectManager::getInstance();
	id = om->generateID();
}

//destructor
StaticObject::~StaticObject(void){
	delete vbomesh;
}

// set normals of vbomesh
// size is the size of array normal
void StaticObject::setNormals(const GLfloat* normal, int size){
	vbomesh->setNormals(normal,size,GL_STATIC_DRAW_ARB);
}

// set texture of vbomesh
// size is the size of array texture, texid is the texture to use
void StaticObject::setTextures(const GLfloat* texture, int size, GLuint texid){
	vbomesh->setTextures(texture,size,GL_STATIC_DRAW_ARB,texid);
}

//set the material of vbomesh
void StaticObject::setMaterial(const GLfloat am[4], const GLfloat di[4], const GLfloat sp[4],
							   const GLfloat em[4], GLfloat sh){
    vbomesh->setMaterial(am,di,sp,em,sh);
}
//set matrix of vbomesh to the given ma
void StaticObject::setMatrix(const float ma[16]){
	vbomesh->setMatrix(ma);
}

// return a list of VBOMeshes that this VBOObject represent
// this function return a pointer to the first VBOMesh of the 
// list and num will be changed to the size of the list
VBOMesh* StaticObject::representInVBOMesh(int* num){
	// set num to 0
	*num = 0;
	return vbomesh;
}

//return the type of this object
int StaticObject::getType(){
	return type;
}

//return the id of this object
int StaticObject::getID(){
	return id;
}
