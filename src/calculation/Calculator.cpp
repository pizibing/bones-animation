#include "Calculator.h"
#include "../data/Mesh.h"
#include "../managers/MeshManager.h"
#include "../managers/SimpleMeshManager.h"

Calculator::Calculator(void){
	meshManager = SimpleMeshManager::getInstance();
}

Calculator::~Calculator(void)
{
}

void Calculator::rotate(float angle,int axis){
	Mesh* mesh = meshManager->getMesh(0,0);
	glPushMatrix();
	glLoadMatrixf(mesh->matrix);
	switch(axis){
		case AXIS_X:
			glRotatef(angle,1,0,0);
			break;
		case AXIS_Y:
			glRotatef(angle,0,1,0);
			break;
		case AXIS_Z:
			glRotatef(angle,0,0,1);
			break;
		default:
			return;
	}
	glGetFloatv(GL_MODELVIEW_MATRIX,mesh->matrix);
	glPopMatrix();
}
