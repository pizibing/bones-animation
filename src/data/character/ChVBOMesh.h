#pragma once

class VBOMesh;

// this class is the CharacterObject version VBOMesh
// it not only stores the VBOMesh but also the vertices
// the this VBOMesh is related to
class ChVBOMesh
{
public:
	// constructor
	// create a VBOMesh , set its vertices to default
	// position of vertices whose ids is in vertices
	ChVBOMesh(const int* vertices, int size);
	// destructor
	~ChVBOMesh(void);

	// get function of vbomesh
	VBOMesh* getVBOMesh();

	// update the vertices of VBOMesh according to the ids
	// contained in vertices.
	// ChVertexInstance is used here as the vertex the id 
	// relates to.
	void updateVBO();

private:
	// the VBOMesh this class packed
	VBOMesh* vbomesh;
	// vertices' ids
	int* vertices;
	// size of vertices, vbomesh's size should be 3 times of size
	int size;
};
