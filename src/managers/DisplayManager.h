#pragma once

#include <vector>

class VBOObject;
class ObjectManager;

// DisplayManager filter all the VBOObjects in ObjectManager
// and decide which objects to display
class DisplayManager
{
public:
	// constructor
	DisplayManager(void);
	// destructor
	~DisplayManager(void);

	// get displayed VBOObjects from ObjectManager
	// this function filter nothing , it returns all the objects
	// in the ObjectManager
	std::vector<VBOObject*>* getDisplayedObjectsAll();

	// get displayed character object from ObjectManager
	// num is the character's id in ObjectManager's character vector
	VBOObject* getDisplayedCharacter(int num);

private:
	ObjectManager* objectManager;
};
