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
