#pragma once

class VBOObject;

// StaticObject is a kind of VBOObject
// its type is OBJECT_TYPE_STATIC
// this kind of object is constant, usually it will not change
// between frames. e.g. a undestroyable building
// StaticObject has no complicate structures, it can be 
// represented by only one VBOMesh
class CharacterObject : public VBOObject
{
public:
	CharacterObject(void);
	~CharacterObject(void);
};
