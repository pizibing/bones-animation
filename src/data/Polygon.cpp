#include "Polygon.h"

Polygon::Polygon(void){
	vertices = new std::vector<Vector3D*>();

	verticeNormals = new std::vector<Vector3D*>();
	hasNormals = false;

	hasTexture = false;
	isTexture2D = true;
	verticeTexCoords = new std::vector<Vector3D*>();

	hasMaterial = false;
}

Polygon::~Polygon(void){
	delete vertices;
	delete verticeNormals;
	delete verticeTexCoords;
}
