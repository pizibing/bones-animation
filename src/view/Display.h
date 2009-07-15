#pragma once

#include <vector>

class Mesh;
class VBOMesh;
class SimpleLine;

// Display is a interface which defines the display functions
// the parameters(e.g Mesh,VBOMesh) given in those functions will be displayed
// in the openGL
class Display
{
public:

	// display a set of VBOMesh
	// toDisplay specify the VBOMeshes to display
	// num is the number of VBOMesh to display
	// isWire specify whether to display in wire model or solid model
	// vertices in VBOMesh will be treated as triangles
	virtual void display(bool isWire,const VBOMesh* toDisplay,int num) = 0;

	// display a group of lines
	// toDisplay specifies the array of lines to display
	// num is the length of this array
	virtual void display(const SimpleLine* toDisplay, int num) = 0;
};