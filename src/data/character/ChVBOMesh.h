#pragma once

class VBOMesh;
class ChSkin;
class ChSkinInstance;

// this class is the CharacterObject version VBOMesh
// it not only stores the VBOMesh but also the vertices
// the this VBOMesh is related to
class ChVBOMesh
{
public:
	// constructor
	// create a VBOMesh , set its vertices to default
	// position of vertices whose ids is in vertices
	// size is the length of vertices
	ChVBOMesh(const int* vertices, int size, ChSkin* skin);
	// destructor
	~ChVBOMesh(void);

	// get function of vbomesh
	VBOMesh* getVBOMesh();

	// update the vertices of VBOMesh according to the ids
	// contained in vertices.
	// ChVertexInstance is used here as the vertex the id 
	// relates to.
	void updateVBO(ChSkinInstance* skin);

private:
	// the VBOMesh this class packed
	VBOMesh* vbomesh;
	// vertices' ids
	int* verticesId;
	// store the position of vertices in the following format
	// x1,y1,z1,x2,y2,z2,x3,y3,z3.....
	float* vertices;
	// store the position of vertices in the following format
	// x1,y1,z1,x2,y2,z2,x3,y3,z3.....
	float* normals;
	// whether this vbomesh has normal
	bool hasNormal;
	// size of vertices, vbomesh's size should be 3 times of size
	int size;
};
