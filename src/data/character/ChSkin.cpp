#include <assert.h>
#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChVertex.h"
#include "ChSkin.h"

// constructor
ChSkin::ChSkin(ChSkeleton* skeleton){
	this->skeleton = skeleton;
	vertices = 0;
}

// destructor
ChSkin::~ChSkin(void){
	if(vertices) delete[] vertices;
	vertices = 0;
}

// initialize vertices and verticeSize
// if vertices has never been initialized, a new array
// of ChVertex will be created whose length is num
// else nothing will happen
void ChSkin::initVertices(int num){
	// num should be bigger than 0
	assert(num > 0);

	// if vertices has been initialized
	if(vertices) return;
	// else
	vertices = new ChVertex*[num];
	verticeSize = num;
}

// set the verNum-th ChVertex of vertices to vertex
// verNum should be smaller than verticeSize
// vertex's id will be set to verNum
ChVertex* ChSkin::initVertice(int verNum){
	// verNum should be smaller than num
	assert(verNum < verticeSize);

	// init ChVertex
	ChVertex* vertex = new ChVertex(skeleton);
	// set id
	vertex->setId(verNum);
	vertices[verNum] = vertex;

	return vertex;
}

// get function of vertices
ChVertex** ChSkin::getVertices(){
	return vertices;
}

// get function of verticeSize
int ChSkin::getVerticeSize(){
	return verticeSize;
}

// return the vertex with the given id
ChVertex* ChSkin::getVertex(int id){
	// because id of vertex is set to its position in the vertices
	return vertices[id];
}