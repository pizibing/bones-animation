#pragma once

class Display;

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

	/*
	* para: toDisplay: the point to a list of Mesh* to be displayed
	*       isWire: true:wire, false:solid
	* func: draw all the meshes in the openGL world space in wire/solid model
	* return: void
	*/
	void display(bool isWire,const std::vector<Mesh*>* toDisplay);
	// display a set of VBOMesh
	// toDisplay specify the VBOMeshes to display
	// num is the number of VBOMesh to display
	// isWire specify whether to display in wire model or solid model
	// vertices in VBOMesh will be treated as triangles
	void display(bool isWire,const VBOMesh* toDisplay,int num);

private:
	// used by void display(bool isWire,const std::vector<Mesh*>* toDisplay);
	// display Mesh in wire model
	void displayWire(const std::vector<Mesh*>* toDisplay);
	// used by void display(bool isWire,const std::vector<Mesh*>* toDisplay);
	// display Mesh in solid model
	void displaySolid(const std::vector<Mesh*>* toDisplay);
};
