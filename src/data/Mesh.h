#pragma once

#include "MyPolygon.h"
#include <vector>

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	std::vector<MyPolygon*>* polygons;
	float matrix[16]; //a matrix record the position and rotation
};
