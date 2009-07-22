#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "ChSkinInstance.h"
#include "../data/character/ChVertex.h"
#include "../data/character/ChSkin.h"
#include "../data/character/ChVertexInstance.h"
#include "../data/character/ChBoneInstance.h"
#include "../data/character/ChSkeletonInstance.h"

// constructor
// this constructor will create ChVertexInstance as many as
// vertices in skin.
ChSkinInstance::ChSkinInstance(ChSkin* skin){
	// get skin information
	ChVertex** ver = skin->getVertices();
	int size = skin->getVerticeSize();
	// create ChSkinInstance's vertices
	vertices = new ChVertexInstance*[size];
	verticeSize = size;
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
			// get related bone id
			int boneId = pairs[j].boneId;
			// get related bone's instance
			ChBoneInstance* boneInstance = skeletonInstance->getBoneInstance(boneId);
			// get related bone's current transform matrix
			Matrix current = Matrix();
			current.set(boneInstance->getAbsoluteRotation(),
				boneInstance->getAbsoluteTranslation());
			// get related bone's default inverse transform matrix
			Matrix inverse = boneInstance->getInverse();
			// calculate current*default inverse
			Matrix mul = current * inverse;
			// change position via current*default inverse
			float* p = vertex->getPosition();
			float ptemp[3];
			for(int k = 0; k < 3; k++)
				ptemp[k] = p[k];
			mul.transform(ptemp);
			// sum position by factor power
			for(int k = 0; k < 3; k++)
				position[k] += ptemp[k] * pairs[j].power;
			// change normal via current*default inverse
			if(vertexInstance->getHasNormal()){
				float* n = vertex->getNormal();
				float ntemp[3];
				for(int k = 0; k < 3; k++)
					ntemp[k] = n[k];
				mul.transform(ntemp);
				// sum normal by factor power
				for(int k = 0; k < 3; k++)
					normal[k] += ntemp[k] * pairs[j].power;
			}
		}
		// set position and normal
		vertexInstance->setPosition(position);
		if(vertexInstance->getHasNormal())
			vertexInstance->setNormal(normal);
	}
}

// get the vertex instance with the given id
ChVertexInstance* ChSkinInstance::getVertex(int id){
	return vertices[id];
}