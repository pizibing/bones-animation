#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "ObjectManager.h"
#include <assert.h>

// initialize singleton to null
ObjectManager* ObjectManager::singleton = NULL;

// constructor
ObjectManager::ObjectManager(void){
	for(int i = 0; i < OBJECT_TYPE_NUM; i++){
		objects[i] = new std::vector<VBOObject*>();
	}
}

// destructor
ObjectManager::~ObjectManager(void){
	for(int i = 0; i < OBJECT_TYPE_NUM; i++)
		delete objects[i];
}

// this function returns the only instance of ObjectManager
// if ObjectManager has never been used before calling this
// function, a new ObjectManager instance will be built by
// using the private constructor
ObjectManager* ObjectManager::getInstance(){
	// if singleton is null, new one
	if(singleton == NULL){
		singleton = new ObjectManager();
	}
	// return singleton
	return singleton;
}

// add a VBOObject to objects
// the vboobject will be stored in type-th vector of objects
// (type-th: type is the type of vboobject)
// vboobject should not be null
void ObjectManager::addVBOObject(VBOObject* vboobject){
	// vboobject should not be null
	assert(vboobject);
	// vboobject's type should not be bigger than OBJECT_TYPE_NUM
	assert(vboobject->getType() < OBJECT_TYPE_NUM);

	// add vboobject to type-th vector of objects
	objects[vboobject->getType()]->push_back(vboobject);
}

// delete VBOObject whose type and id is the same as vboobject
// from objects
// vboobject should not be null
void ObjectManager::deleteVBOObject(VBOObject* vboobject){
	// vboobject should not be null
	assert(vboobject);
	// vboobject's type should not be bigger than OBJECT_TYPE_NUM
	assert(vboobject->getType() < OBJECT_TYPE_NUM);

	// get all the vbo objects with the type
	std::vector<VBOObject*>* obs = objects[vboobject->getType()];
	// for each vbo object
	for(std::vector<VBOObject*>::iterator it = obs->begin(); it < obs->end(); it++){
		// if its id is the same as vboobject, delete it
		if((*it)->getID() == vboobject->getID()){
			obs->erase(it);
			return;
		}
	}
}

// return a VBOObject that stored in objects and whose type
// and id are the same as given type and id
// if there's no match, return NULL(0)
// type should not bigger than OBJECT_TYPE_NUM
VBOObject* ObjectManager::getVBOObject(int type, int id){
	// type should not be bigger than OBJECT_TYPE_NUM
	assert(type < OBJECT_TYPE_NUM);

	// get all the vbo objects with the type
	std::vector<VBOObject*>* obs = objects[type];
	// for each vbo object
	for(std::vector<VBOObject*>::iterator it = obs->begin(); it < obs->end(); it++){
		// if its id is the same as vboobject
		if((*it)->getID() == id){
			return *it;
		}
	}
	// find no match
	return NULL;
}

// return a pointer to a vector which contains all the 
// VBOObjects stored in objects and type is the given type
// type should not bigger than OBJECT_TYPE_NUM
std::vector<VBOObject*>* ObjectManager::getVBOObjects(int type){
	// type should not be bigger than OBJECT_TYPE_NUM
	assert(type < OBJECT_TYPE_NUM);

	// get all the vbo objects with the type
	std::vector<VBOObject*>* obs = objects[type];

	return obs;
}

// generate a unique id for a VBOObject
int ObjectManager::generateID(){

	// initialize id
	static int id = -1;
	// id should be unique
	id++;

	return id;
}
