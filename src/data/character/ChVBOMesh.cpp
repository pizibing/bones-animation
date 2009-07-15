#include "../matrixlib/mtxlib.h"
#include "../data/VBOMesh.h"
#include "ChVBOMesh.h"
#include "../data/character/ChVertex.h"
#include "../data/character/ChSkin.h"
#include "../data/character/ChVertexInstance.h"
#include "../data/character/ChSkinInstance.h"

// constructor
// create a VBOMesh , set its vertices to default
// position of vertices whose ids is in vertices
// size is the length of vertices
ChVBOMesh::ChVBOMesh(const int* vertices, int size, ChSkin* skin){
	// because vertices should construct triangles
	// size can be divided exactly by 3
	int remain = size % 3;
	assert(remain == 0);
	// vertices should not be null
	assert(vertices);

	// set size
	this->size = size * 3;


	// set normal
	// decide whether this vbomesh has normal via first vertex
	if(skin->getVertex(vertices[0])->getHasNormal()){
		hasNormal = true;
		// initialize normals
		normals = new float[this->size];
	}
	else{
		hasNormal = false;
	}

	// set vertices
	this->verticesId = new int[size];
	for(int i = 0; i < size; i++)
		this->verticesId[i] = vertices[i];
	// build default vertices according to the ids given
	this->vertices = new float[this->size];

	// for each id
	for(int i = 0; i < size; i++){
		ChVertex* vertex = skin->getVertex(vertices[i]);
		float* position =  vertex->getPosition();
		// set position x,y,z into vertices
		this->vertices[3 * i + 0] = position[0];
		this->vertices[3 * i + 1] = position[1];
		this->vertices[3 * i + 2] = position[2];

		float* normal = vertex->getNormal();
		// set normals into normals
		if(hasNormal){
			normals[3 * i + 0] = normal[0];
			normals[3 * i + 1] = normal[1];
			normals[3 * i + 2] = normal[2];
		}
	}

	// initialize vbomesh
	// because of vertices will change every frame, so use
	// GL_DYNAMIC_DRAW_ARB 
	vbomesh = new VBOMesh(this->vertices,this->size,GL_DYNAMIC_DRAW_ARB);
	if(hasNormal) vbomesh->setNormals(normals,this->size,GL_DYNAMIC_DRAW_ARB);
}

ChVBOMesh::~ChVBOMesh(void){
	if(verticesId) delete[] verticesId;
	if(vertices) delete[] vertices;
	if(normals) delete[] normals;
	if(vbomesh) delete vbomesh;
}

// get function of vbomesh
VBOMesh* ChVBOMesh::getVBOMesh(){
	return vbomesh;
}

// update the vertices of VBOMesh according to the ids
// contained in vertices.
// ChVertexInstance is used here as the vertex the id 
// relates to.
void ChVBOMesh::updateVBO(ChSkinInstance* skin){
	// update vertices
	int vsize = size / 3;
	// for each id
	for(int i = 0; i < vsize; i++){
		ChVertexInstance* vertex = skin->getVertex(verticesId[i]);
		float* position =  vertex->getPosition();
		// update positions
		vertices[3 * i + 0] = position[0];
		vertices[3 * i + 1] = position[1];
		vertices[3 * i + 2] = position[2];

		if(hasNormal){
			// update normals
			float* normal = vertex->getNormal();
			normals[3 * i + 0] = normal[0];
			normals[3 * i + 1] = normal[1];
			normals[3 * i + 2] = normal[2];
		}
	}

	// update vbomesh
	vbomesh->updateVertices(vertices,size);
	if(hasNormal) vbomesh->updateNormals(normals,size);
}

// update the matrix of the VBOMesh to the given matrix
void ChVBOMesh::updateVBO(float* matrix){
	// matrix should not be null
	assert(matrix);

	vbomesh->setMatrix(matrix);
}
