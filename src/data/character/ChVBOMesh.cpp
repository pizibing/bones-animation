#include "ChVBOMesh.h"

ChVBOMesh::ChVBOMesh(const int* vertices, int size)
{
}

ChVBOMesh::~ChVBOMesh(void)
{
}

// get function of vbomesh
VBOMesh* ChVBOMesh::getVBOMesh(){
	return 0;
}

// update the vertices of VBOMesh according to the ids
// contained in vertices.
// ChVertexInstance is used here as the vertex the id 
// relates to.
void ChVBOMesh::updateVBO(){}
