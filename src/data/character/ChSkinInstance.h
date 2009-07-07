#pragma once

// ChSkinInstance is a class that stores the current instance
// of a character's skin(vertices' positions and normals).
// this instance will be changed every frame according to the
// move of ChBoneInstance.
class ChSkinInstance
{
public:
	// constructor
	ChSkinInstance(void);
	// destructor
	~ChSkinInstance(void);
};
