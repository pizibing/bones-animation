#pragma once

#include <vector>

// define the number of types of VBOObject
// this number should not be little than the biggest VBOObject typeNum defined
const int OBJECT_TYPE_NUM = 3;

class VBOObject;

// ObjectManager is a singleton class that manage VBOObjects
// it stores all the VBOObjects in this system
// each type of VBOObject will be stored in a vector whose id 
// is the same as type
// it provides add, delete, get functions for VBOObjects
class ObjectManager
{
public:
	// this function returns the only instance of ObjectManager
	// if ObjectManager has never been used before calling this
	// function, a new ObjectManager instance will be built by
	// using the private constructor
	static ObjectManager* getInstance();

	// add a VBOObject to objects
	// the vboobject will be stored in type-th vector of objects
	// (type-th: type is the type of vboobject)
	// vboobject should not be null
	void addVBOObject(VBOObject* vboobject);

	// delete VBOObject whose type and id is the same as vboobject
	// from objects
	// vboobject should not be null
	void deleteVBOObject(VBOObject* vboobject);

	// return a VBOObject that stored in objects and whose type
	// and id are the same as given type and id
	// if there's no match, return NULL(0)
	// type should not bigger than OBJECT_TYPE_NUM
	VBOObject* getVBOObject(int type, int id);

	// return a pointer to a vector which contains all the 
	// VBOObjects stored in objects and type is the given type
	// type should not bigger than OBJECT_TYPE_NUM
	std::vector<VBOObject*>* getVBOObjects(int type);

	// generate a unique id for a VBOObject
	static int generateID();

private:
	//constructor
	ObjectManager(void);
	//destructor
	~ObjectManager(void);

	//singleton 
	static ObjectManager* singleton;

	// objects stores all the VBOObjects
	// each type of VBOObject will be stored in a vector whose id 
	// is the same as type
	std::vector<VBOObject*>* objects[OBJECT_TYPE_NUM];
};
