#include "MyPolygon.h"
#include "../matrixlib/Vector3D.h"

MyPolygon::MyPolygon(void){
	vertices = new std::vector<Vector3D*>();

	verticeNormals = new std::vector<Vector3D*>();
	hasNormals = false;

	hasTexture = false;
	isTexture2D = true;
	verticeTexCoords = new std::vector<Vector3D*>();

	hasMaterial = false;
}

MyPolygon::~MyPolygon(void){
	delete vertices;
	delete verticeNormals;
	delete verticeTexCoords;
}
