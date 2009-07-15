#include "VBOMesh.h"
//#include "VBO.h"
#include <iostream>
#include <assert.h>

//functions for VBO
extern PFNGLGENBUFFERSARBPROC pglGenBuffersARB;                     // VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC pglBindBufferARB;                     // VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC pglBufferDataARB;                     // VBO Data Loading Procedure
extern PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB;               // VBO Sub Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB;               // VBO Deletion Procedure
extern PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB; // return various parameters of VBO
extern PFNGLMAPBUFFERARBPROC pglMapBufferARB;                       // map VBO procedure
extern PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB

//constructors
VBOMesh::VBOMesh(void){
	size = 0;
	hasNormal = false;
	normals = 0;
	hasTexture = false;
	textures = 0;
	texID = 0;
	vertices = 0;
	hasMaterial = false;
	//init matrix to identity matrix
	for(int i = 1; i < 16; i++) matrix[i] = 0;
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
}

//generate a VBOMesh whose vertices are defined by the given
//vertex and usage. hasNormal, hasTexture, hasMaterial will be set false.
//matrix will be set to identity 4*4 matrix
//size will be set to the size of vertex divided by 3
//if vertex's size can not be divided exactly by 3, vertices will be set to 0
// By default, usage mode is set as GL_STATIC_DRAW_ARB.
// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
VBOMesh::VBOMesh(const GLfloat* vertex, int size, GLenum usage){
	//vertex should not be null
	assert(vertex != 0);

	hasNormal = false;
	normals = 0;
	hasTexture = false;
	textures = 0;
	texID = 0;
	hasMaterial = false;
	//init matrix to identity matrix
	for(int i = 1; i < 16; i++) matrix[i] = 0;
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	//init vertex
	int remain = size%3;
	//check the size of vertex, size should be able to be divided by 3
	assert(remain == 0);
	vertices = createVBO(vertex,size*sizeof(GLfloat),GL_ARRAY_BUFFER_ARB,usage);
	//set size
	this->size = size/3;
}

//destructor
VBOMesh::~VBOMesh(void){}

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
void VBOMesh::setVertices(const GLfloat* vertex, int size, GLenum usage){
	//vertex should not be null
	assert(vertex != 0);
	//vertices should be 0
	assert(vertices == 0);

	//set vertices
	int remain = size%3;
	//check the size of vertex, size should be able to be divided by 3
	assert(remain == 0);
	vertices = createVBO(vertex,size*sizeof(GLfloat),GL_ARRAY_BUFFER_ARB,usage);
	//set size
	this->size = size/3;
}

//if normals of this VBOMesh is 0 (no VBO is generated for normals)
//then generate a VBO using normal and usage, hasNormal will be set to true
//else nothing will happen
//if normal's size doesn't equal to size*3, nothing will happen
// By default, usage mode is set as GL_STATIC_DRAW_ARB.
// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
void VBOMesh::setNormals(const GLfloat* normal, int size, GLenum usage){
	//normal should not be null
	assert(normal);
	//normals should be 0
	assert(normals == 0);

	//check normal's size
	//normal's size should equal to size*3
	assert(size == this->size*3);
	//set normals
	normals = createVBO(normal,size*sizeof(GLfloat),GL_ARRAY_BUFFER_ARB,usage);
	//set hasNormal
	hasNormal = true;
}

//if textures of this VBOMesh is 0 (no VBO is generated for textures)
//then generate a VBO using texture and usage, hasTexture will be set to true
//texID will be set to texid
//else nothing will happen
//if texture's size doesn't equal to size*2, nothing will happen
// By default, usage mode is set as GL_STATIC_DRAW_ARB.
// Other usages are GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, GL_STREAM_COPY_ARB,
// GL_STATIC_DRAW_ARB, GL_STATIC_READ_ARB, GL_STATIC_COPY_ARB,
// GL_DYNAMIC_DRAW_ARB, GL_DYNAMIC_READ_ARB, GL_DYNAMIC_COPY_ARB.
void VBOMesh::setTextures(const GLfloat* texture, int size, GLenum usage, GLuint texid){
	//texture should not be null
	assert(texture);
	//normals should be 0
	assert(textures == 0);

	//check texture's size
	//texture's size should equal to size*3
	assert(size == this->size*2);
	//set normals
	textures = createVBO(texture,size*sizeof(GLfloat),GL_ARRAY_BUFFER_ARB,usage);
	//set texID
	texID = texid;
	//set hasNormal
	hasTexture = true;
}

//set the material of this VBOMesh.
//hasMaterial will be set to true and
//ambient,diffuse,specular,emission,shininess will be set
void VBOMesh::setMaterial(const GLfloat am[4], const GLfloat di[4], const GLfloat sp[4],
						  const GLfloat em[4], GLfloat sh){
    
	//set material
	for(int i = 0; i < 4; i++){
		ambient[i] = am[i];
		diffuse[i] = di[i];
		specular[i] = sp[i];
		emission[i] = em[i];
	}
	shininess = sh;
	//set hasMaterial
	hasMaterial = true;
}

//set matrix of this VBOMesh to the given ma
void VBOMesh::setMatrix(const float ma[16]){
	for(int i = 0; i < 16; i++)
		matrix[i] = ma[i];
}

//update vertices VBO to the version of vertex
//GL_WRITE_ONLY_ARB will be set as the map state
//if vertices equals 0, nothing will happen
//if vertex's size doesn't equal to size*3, nothing will happen
void VBOMesh::updateVertices(const GLfloat* vertex, int size){
	//vertex should not be null
	assert(vertex);
	//check size of vertex
	assert(size == this->size*3);
	//check vertices
	assert(vertices != 0);

	//update
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vertices);
	float* vbo = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB);
	updateVBO(vbo,(float*)vertex,size);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

//update normals VBO to the version of normal
//GL_WRITE_ONLY_ARB will be set as the map state
//if normals equals 0, nothing will happen
//if normal's size doesn't equal to size*3, nothing will happen
void VBOMesh::updateNormals(const GLfloat* normal, int size){
	//normal should not be null
	assert(normal);
	//check size of normal
	assert(size == this->size*3);
	//check normals
	assert(normals != 0);

	//update
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,normals);
	float* vbo = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB);
	updateVBO(vbo,(float*)normal,size);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

//update textures VBO to the version of texture
//GL_WRITE_ONLY_ARB will be set as the map state
//if textures equals 0, nothing will happen
//if texture's size doesn't equal to size*2, nothing will happen
void VBOMesh::updateTextures(const GLfloat* texture, int size){
	//texture should not be null
	assert(texture);
	//check size of texture
	assert(size == this->size*2);
	//check textures
	assert(textures != 0);

	//update
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,textures);
	float* vbo = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB);
	updateVBO(vbo,(float*)texture,size);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

//get functions
// return hasNormal
bool VBOMesh::getHasNormal(){
	return hasNormal;
}

// return normals
GLuint VBOMesh::getNormals(){
	return normals;
}

// return hasTexture
bool VBOMesh::getHasTexture(){
	return hasTexture;
}

// return textures
GLuint VBOMesh::getTextures(){
	return textures;
}

// return texID
GLuint VBOMesh::getTexID(){
	return texID;
}

// return vertices
GLuint VBOMesh::getVertices(){
	return vertices;
}

// return hasMaterial
bool VBOMesh::getHasMaterial(){
	return hasMaterial;
}

// return ambient[4]
GLfloat* VBOMesh::getAmbient(){
	return ambient;
}

// return diffuse[4]
GLfloat* VBOMesh::getDiffuse(){
	return diffuse;
}

// return specular[4]
GLfloat* VBOMesh::getSpecular(){
	return specular;
}

// return emission[4]
GLfloat* VBOMesh::getEmission(){
	return emission;
}

// return shininess
GLfloat VBOMesh::getShininess(){
	return shininess;
}

// return matrix[16]
float* VBOMesh::getMatrix(){
	return matrix;
}

int VBOMesh::getSize(){
	return size;
}

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
GLuint VBOMesh::createVBO(const void* data, int dataSize, GLenum target, GLenum usage){
	GLuint id = 0;  // 0 is reserved, glGenBuffersARB() will return non-zero id if success

	glGenBuffersARB(1, &id);                        // create a vbo
	glBindBufferARB(target, id);                    // activate vbo id to use
	glBufferDataARB(target, dataSize, data, usage); // upload data to video card

	// check data size in VBO is same as input array, if not return 0 and delete VBO
	int bufferSize = 0;
	glGetBufferParameterivARB(target, GL_BUFFER_SIZE_ARB, &bufferSize);
	if(dataSize != bufferSize)
	{
		glDeleteBuffersARB(1, &id);
		id = 0;
		std::cout << "[createVBO()] Data size is mismatch with input array\n";
	}

	return id;      // return VBO id
}

//update the vbo to the version of src
//i.e. every element of vbo equals corresponding element of src
void VBOMesh::updateVBO(float* vbo, float* src, int count){
	//vbo should not be null pointer
	assert(vbo != 0);
	//src should not be null pointer
	assert(src != 0);
	//count should be bigger than 0
	assert(count > 0);

	//update vbo
	for(int i = 0; i < count; i++){
		*vbo = *src;
		vbo++;
		src++;
	}
}
