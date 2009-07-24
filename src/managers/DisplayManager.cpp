#include <assert.h>
#include "DisplayManager.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../managers/ObjectManager.h"

// constructor
DisplayManager::DisplayManager(void){
	objectManager = ObjectManager::getInstance();
}

// destructor
DisplayManager::~DisplayManager(void){
}

// get displayed VBOObjects from ObjectManager
// this function filter nothing , it returns all the objects
// in the ObjectManager
std::vector<VBOObject*>* DisplayManager::getDisplayedObjectsAll(){
	std::vector<VBOObject*>* result = new std::vector<VBOObject*>();
	// for each type of objects
	for(int i = 0; i < OBJECT_TYPE_NUM; i++){
		std::vector<VBOObject*>* objects = objectManager->getVBOObjects(i);
		// for each object in this type
		for(int j = 0; j < (signed int)objects->size(); j++){
			// push it into result
			result->push_back((*objects)[j]);
		}
	}

	return result;
}

// get displayed character object from ObjectManager
// num is the character's id in ObjectManager's character vector
VBOObject* DisplayManager::getDisplayedCharacter(int num){
	// get characters
	std::vector<VBOObject*>* characters =
		objectManager->getVBOObjects(OBJECT_TYPE_CHARACTER);

	// num should be smaller than characters's size
	assert(num < characters->size());

	return (*characters)[num];
}
