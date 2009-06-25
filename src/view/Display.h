#pragma once

#include "Mesh.h"
#include <vector>

class Display
{
public:
	/*
	 * para: toDisplay: the point to a list of Mesh* to be displayed
	 *       isWire: true:wire, false:solid
	 * func: draw all the meshes in the openGL world space in wire/solid model
	 * return: void
	 */
	virtual void display(bool isWire,const std::vector<Mesh*>* toDisplay) = 0;
};