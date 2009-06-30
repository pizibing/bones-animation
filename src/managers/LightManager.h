#pragma once

#include <gl/glut.h>

#define LIGHT_NUM 10

class LightManager
{
public:
	static LightManager* getInstance();

	GLfloat* getAmbient(int i);
	GLfloat* getDiffuse(int i);
	GLfloat* getSpecular(int i);
	GLfloat* getPosition(int i);

private:
	LightManager(void);
	~LightManager(void);

	GLfloat ambient[LIGHT_NUM][4];
	GLfloat diffuse[LIGHT_NUM][4];
	GLfloat specular[LIGHT_NUM][4];
	GLfloat position[LIGHT_NUM][4];

	static LightManager* singleton;
};
