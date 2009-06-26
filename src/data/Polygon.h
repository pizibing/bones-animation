#pragma once

#include "../matrixlib/Vector3D.h"
#include <vector>
#include <GL/glui.h>

class Polygon
{
public:
	Polygon(void);
	~Polygon(void);

	//vertices
	std::vector<Vector3D*>* vertices;

	//normals
	bool hasNormals;
	std::vector<Vector3D*>* verticeNormals;

	//texture
	bool hasTexture;
	bool isTexture2D;
	std::vector<Vector3D*>* verticeTexCoords;
	GLuint texID;

	//material
	bool hasMaterial;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;
};
