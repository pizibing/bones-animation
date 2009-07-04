#pragma once

// define type 
const int OBJECT_TYPE_CHARACTER = 0; // character VBOObject
const int OBJECT_TYPE_TERRAIN = 1;  // terrain VBOObject
const int OBJECT_TYPE_STATIC = 2; // static VBOObject

// VBOObject is a object that can be represented by a list of 
// VBOMeshes.
// all VBOObject has its own type and a unique id
// type of a VBOObject defines its role, e.g. main character,
// terrain or static object etc.
class VBOObject
{
public:
	// return a list of VBOMeshes that this VBOObject represent
	// this function return a pointer to the first VBOMesh of the 
	// list and num will be changed to the size of the list
	virtual VBOMesh* representInVBOMesh(int* num) = 0;

	//return the type of this object
	virtual int getType() = 0;

	//return the id of this object
	virtual int getID() = 0;

protected:
	// the type of this VBOObject. e.g. main character,
	// terrain or static object etc.
	int type;
	// a unique id among all VBOObject
	int id;
};
