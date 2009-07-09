#pragma once

class VBOObject;
class VBOMesh;

// StaticObject is a kind of VBOObject
// its type is OBJECT_TYPE_STATIC
// this kind of object is constant, usually it will not change
// between frames. e.g. a undestroyable building
// StaticObject has no complicate structures, it can be 
// represented by only one VBOMesh
class StaticObject : public VBOObject
{
public:
	// constructor
	// set vertex of vbomesh, size is the size of array vertex
	StaticObject(const GLfloat* vertex, int size);
	// destructor
	~StaticObject(void);

	// set normals of vbomesh
	// size is the size of array normal
	void setNormals(const GLfloat* normal, int size);
	// set texture of vbomesh
	// size is the size of array texture, texid is the texture to use
	void setTextures(const GLfloat* texture, int size, GLuint texid);
	//set the material of vbomesh
	void setMaterial(const GLfloat am[4], const GLfloat di[4], const GLfloat sp[4],
		const GLfloat em[4], GLfloat sh);
	//set matrix of vbomesh to the given ma
	void setMatrix(const float ma[16]);

	// return a list of VBOMeshes that this VBOObject represent
	// this function return a pointer to the first VBOMesh of the 
	// list and num will be changed to the size of the list
	VBOMesh* representInVBOMesh(int* num);

	//return the type of this object
	int getType();

	//return the id of this object
	int getID();

private:
	// because StaticObject is simple ,it can be represented by
	// only one VBOMesh
	VBOMesh* vbomesh;
};
