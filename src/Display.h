#pragma once

#include "Mesh.h"
#include <vector>

class Display
{
public:
	/*
	 * para: toDisplay: the point to a list of Mesh* to be displayed
	 * func: draw all the meshes in the openGL world space
	 * return: void
	 */
	virtual void display(const std::vector<Mesh*>* toDisplay) = 0;
};