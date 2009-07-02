#include "MyPolygon.h"
#include "Mesh.h"

Mesh::Mesh(void){
	polygons = new std::vector<MyPolygon*>();
	//matrix that indicates no movement or rotation
	for(int i = 1; i < 16; i++) matrix[i] = 0;
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
}

Mesh::~Mesh(void){
	delete polygons;
}
