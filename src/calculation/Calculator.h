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

	// make the character stored in ObjectManager to move 
	// forward in the given direction
	// angle is an angle in degree between -1 and 360
	// angle is the angle from +x axis to the direction
	// if angle is -1, then character remain its direction
	// but the speed will slow down
	// if angle is the same as default character will speed up
	// if angle is different from the character's direction
	// the speed will slow down a little too.
	void moveCharacter(float angle);

private:
	//object manager that stores all the data
	ObjectManager* objectManager;
};
