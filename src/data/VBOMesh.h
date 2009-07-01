#pragma once

#include "gl/glut.h"

//a VBOMesh is all the information of a mesh include position of 
//every vertices, normals, textures, material and matrix
//position, normal, texture information are stored in VBO
class VBOMesh
{
public:
	//constructors
	VBOMesh(void);
	//generate a VBOMesh whose vertices are defined by the given
	//vertex and usage. hasNormal, hasTexture, hasMaterial will be set false.
	//matrix will be set to identity 4*4 matrix
	//size will be set to the size of vertex divided by 3
	//if vertex's size can not be divided exactly by 3, vertices will be set to 0
	// By default, usage mode is set as GL_STATIC_DRAW_ARB.
	// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
	// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
	// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
	VBOMesh(const GLfloat* vertex, GLenum usage);

	//destructor
	~VBOMesh(void);

	//functions
	//if vertices of this VBOMesh is 0 (no VBO is generated for vertices)
	//then generate a VBO using vertex and usage
	//size will be set to the size of vertex divided by 3
	//if vertex's size can not be divided exactly by 3, nothing happen
	//else nothing will happen
	// By default, usage mode is set as GL_STATIC_DRAW_ARB.
	// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
	// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
	// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
	void setVertices(const GLfloat* vertex, GLenum usage);
	//if normals of this VBOMesh is 0 (no VBO is generated for normals)
	//then generate a VBO using normal and usage, hasNormal will be set to true
	//else nothing will happen
	//if normal's size doesn't equal to size*3, nothing will happen
	// By default, usage mode is set as GL_STATIC_DRAW_ARB.
	// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
	// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
	// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
	void setNormals(const GLfloat* normal, GLenum usage);
	//if textures of this VBOMesh is 0 (no VBO is generated for textures)
	//then generate a VBO using texture and usage, hasTexture will be set to true
	//texID will be set to texid
	//else nothing will happen
	//if texture's size doesn't equal to size*2, nothing will happen
	// By default, usage mode is set as GL_STATIC_DRAW_ARB.
	// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
	// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
	// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
	void setTextures(const GLfloat* texture, GLenum usage, GLuint texid);
	//set the material of this VBOMesh.
	//hasMaterial will be set to true and
	//ambient,diffuse,specular,emission,shininess will be set
	void setMaterial(const GLfloat am[4], const GLfloat di[4], const GLfloat sp[4],
		const GLfloat em[4], GLfloat sh);
	//set matrix of this VBOMesh to the given ma
	void setMatrix(const float ma[16]);

	//update vertices VBO to the version of vertex
	//GL_WRITE_ONLY_ARB will be set as the map state
	//if vertices equals 0, nothing will happen
	//if vertex's size doesn't equal to size*3, nothing will happen
	void updateVertices(const GLfloat* vertex);
	//update normals VBO to the version of normal
	//GL_WRITE_ONLY_ARB will be set as the map state
	//if normals equals 0, nothing will happen
	//if normal's size doesn't equal to size*3, nothing will happen
	void updateNormals(const GLfloat* normal);
	//update textures VBO to the version of texture
	//GL_WRITE_ONLY_ARB will be set as the map state
	//if textures equals 0, nothing will happen
	//if texture's size doesn't equal to size*2, nothing will happen
	void updateTextures(const GLfloat* texture);

private:

	// generate vertex buffer object and bind it with its data
	// You must give 2 hints about data usage; target and mode, so that OpenGL can
	// decide which data should be stored and its location.
	// VBO works with 2 different targets; GL_ARRAY_BUFFER_ARB for vertex arrays
	// and GL_ELEMENT_ARRAY_BUFFER_ARB for index array in glDrawElements().
	// The default target is GL_ARRAY_BUFFER_ARB.
	// By default, usage mode is set as GL_STATIC_DRAW_ARB.
	// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
	// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
	// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
	GLuint createVBO(const void* data, int dataSize, GLenum target, GLenum usage);
	//update the first 'count' elements of vbo to the version of src
	//i.e. every element of vbo equals corresponding element of src
	void updateVBO(float* vbo, float* src, int count);


	//the size of vertices and should be the size of normals
	//textures
	int size;

	//whether this VBOMesh has normals
	bool hasNormal;
	//VBO id of normals
	GLuint normals;

	//whether this VBOMesh has texture
	bool hasTexture;
	//VBO id of texture coordinates
	GLuint textures;
	//texture id of the texture
	GLuint texID;

	//VBO id of vertices
	GLuint vertices;

	//whether this VBOMesh has material
	bool hasMaterial;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;

	//a matrix record the position of the VBOMesh in the world space
	float matrix[16];
};
