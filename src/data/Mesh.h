#pragma once

#include "Polygon.h"
#include <vector>

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	std::vector<Polygon*>* polygons;
	float matrix[16]; //a matrix record the position and rotation
};
