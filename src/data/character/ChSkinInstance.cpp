#include "ChSkinInstance.h"
#include "../matrixlib/mtxlib.h"
#include "../data/character/ChVertex.h"
#include "../data/character/ChSkin.h"
#include "../data/character/ChVertexInstance.h"

// constructor
// this constructor will create ChVertexInstance as many as
// vertices in skin.
ChSkinInstance::ChSkinInstance(ChSkin* skin){
	// get skin information
	ChVertex** ver = skin->getVertices();
	int size = skin->getVerticeSize();
	// create ChSkinInstance's vertices
	vertices = new ChVertexInstance*[size];
	verticeSize = verticeSize;
	// for each ChVertex, create corresponding ChVertexInstance
	for(int i = 0; i < size; i++){
		ChVertex* vertex = ver[i];
		// init
		vertices[i] = new ChVertexInstance(vertex->getId());
		// set position
		vertices[i]->setPosition(vertex->getPosition());
		// if has normal
		if(vertex->getHasNormal()){
			vertices[i]->setHasNormal(vertex->getHasNormal());
			// set normal
			vertices[i]->setNormal(vertex->getNormal());
		}
	}
}

// destructor
ChSkinInstance::~ChSkinInstance(void){
	if(verticeSize > 0){
		for(int i = 0; i < verticeSize; i++)
			delete vertices[i];
	}
	if(vertices) delete[] vertices;
}

// calculate all the vertex instance according to the given
// skeleton instance
void ChSkinInstance::calSkinInstance(ChSkeletonInstance* skeletonInstance, ChSkin* skin){
	// for each vertex instance
	for(int i = 0; i < verticeSize; i++){
		ChVertexInstance* vertexInstance = vertices[i];
		// get corresponding vertex in skin
		ChVertex* vertex = skin->getVertex(vertexInstance->getId());
		// calculate current position and normal
		float position[3];
		float normal[3];
		// get pair
		VBpair* pairs = vertex->getPairs();
		int pairNum = vertex->getPairNum();
		// for each pair
		for(int j = 0; j < pairNum; j++){
			//calculate position and normal, multiply power,and add them to sum
		}
		// set position and normal
		vertexInstance->setPosition(position);
		vertexInstance->setNormal(normal);
	}
}