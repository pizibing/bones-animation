#pragma once

class ChVertexInstance;
class ChSkin;
class ChSkeletonInstance;

// ChSkinInstance is a class that stores the current instance
// of a character's skin(all the ChVertexInstance).
// this instance will be changed every frame according to the
// move of ChSkeletonInstance.
class ChSkinInstance
{
public:
	// constructor
	// this constructor will create ChVertexInstance as many as
	// vertices in skin.
	ChSkinInstance(ChSkin* skin);
	// destructor
	~ChSkinInstance(void);

	// calculate all the vertex instance according to the given
	// skeleton instance
	void calSkinInstance(ChSkeletonInstance* skeletonInstance, ChSkin* skin);

private:
	// all the ChVertexInstances
	ChVertexInstance** vertices;
	// size of vertices
	int verticeSize;
};
