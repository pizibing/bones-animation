#pragma once

#include "../managers/MeshManager.h"
#include "../managers/SimpleMeshManager.h"
#include <gl/glut.h>

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class Calculator
{
public:
	Calculator(void);
	~Calculator(void);

	void rotate(float angle,int axis);

private:
	MeshManager* meshManager;
};
