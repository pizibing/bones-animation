#include "Mesh.h"

Mesh::Mesh(void){
	polygons = new std::vector<Polygon*>();
}

Mesh::~Mesh(void){
	delete polygons;
}
