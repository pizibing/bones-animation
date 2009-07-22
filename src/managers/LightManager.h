#pragma once

#include <gl/glut.h>

// number of lights that this manager controls
const int LIGHT_NUM = 10;

// LightManager manages all the information
// (ambient,diffuse,specular,position) of every light
// it controls.
// it only manages those information. if you want to
// set a light in openGL, you should get information
// from this manager and set it into openGL by yourself.
class LightManager
{
public:
	// get the single instance of LightManager
	static LightManager* getInstance();

	// get the ambient of the i-th light
	GLfloat* getAmbient(int i);
	// get the diffuse of the i-th light
	GLfloat* getDiffuse(int i);
	// get the specular of the i-th light
	GLfloat* getSpecular(int i);
	// get the position of the i-th light
	GLfloat* getPosition(int i);

private:
	// private constructor
	LightManager(void);
	// destructor
	~LightManager(void);

	// two dimension arrays that store information of the lights
	// first dimension defines which light owns the information
	// second dimension stores the information
	GLfloat ambient[LIGHT_NUM][4];
	GLfloat diffuse[LIGHT_NUM][4];
	GLfloat specular[LIGHT_NUM][4];
	GLfloat position[LIGHT_NUM][4];

	// singleton
	static LightManager* singleton;
};
