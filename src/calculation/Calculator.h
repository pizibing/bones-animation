#pragma once

#include <gl/glut.h>

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class ObjectManager;

// Calculator take orders from Command, then
// get the source data from ObjectManager and
// change some of the data according to the order(techniques
// like physics, animation etc. may be taken in consideration)
class Calculator
{
public:
	//constructor
	Calculator(void);
	//destructor
	~Calculator(void);

private:
	//object manager that stores all the data
	ObjectManager* objectManager;
};
