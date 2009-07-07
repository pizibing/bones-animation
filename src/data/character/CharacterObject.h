#pragma once

class VBOMesh;
class VBOObject;
class ChBoneInstance;
class ChSkinInstance;

// CharacterObject is a kind of VBOObject
// its type is OBJECT_TYPE_CHARACTER
// this kind of VBOObject is usually an object with a skeleton,
// some animations of the skeleton and skin.
// it changes from frame to frame according to its animation,
// so its instance must be update every single frame.
// it is always represented by several VBOMeshes
class CharacterObject : public VBOObject
{
public:
	// constructor
	CharacterObject(void);
	// destructor
	~CharacterObject(void);

	// return a list of VBOMeshes that this VBOObject represent
	// this function return a pointer to the first VBOMesh of the 
	// list and num will be changed to the size of the list
	VBOMesh* representInVBOMesh(int* num);

	//return the type of this object
	int getType();

	//return the id of this object
	int getID();

private:
	// a pointer to an array that stores all the VBOMeshes of this 
	// object represents.
	// they should be updated every frame according to the matrixInstance,
	// chBoneInstance, chSkinInstance
	VBOMesh* vbomeshes;
	// matrix instance that may be changed every frame
	float chMatrixInstance[];
	// bone instance that may be changed every frame
	ChBoneInstance* chBoneInstance;
	// skin instance that may be changed every frame
	ChSkinInstance* chSkinInstance;
};
