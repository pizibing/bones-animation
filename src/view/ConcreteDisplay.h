#pragma once

class Display;
class SimpleLine;

// ConcreteDisplay realize Display interface
// it is some concrete solution to display objects(Mesh,VBOMesh)
// in openGL
class ConcreteDisplay : public Display
{
public:
	//constructor
	ConcreteDisplay(void);
	//destructor
	~ConcreteDisplay(void);

	// display a set of VBOMesh
	// toDisplay specify the VBOMeshes to display
	// num is the number of VBOMesh to display
	// isWire specify whether to display in wire model or solid model
	// vertices in VBOMesh will be treated as triangles
	void display(bool isWire,const VBOMesh* toDisplay,int num);

	// display a group of lines
	// toDisplay specifies the array of lines to display
	// num is the length of this array
	void display(const SimpleLine* toDisplay, int num);
};
