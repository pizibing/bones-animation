#pragma once

#include <gl/glut.h>

class VBOMesh;
class VBOObject;
class VBOMesh;

// define the type id of material used
const int TERRAIN_MATERIAL_TYPE_NUM = 2;
const int SOIL_TERRAIN = 0;
const int METAL_TERRAIN = 1;

// TerrainObject is an VBOObject that represent a terrain
// terrain is stored in a vbomesh which contains all the 
// triangles that construct this terrain.
// the input of a TerrainObject should be the height and texture id
// position, normal and texture coordinate should be calculated
// accordingly.
// also user can choose a material for the terrain, e.g.
// soil, metal etc.
// initial step: 1. constructor 2. setTexId 3. setHeightMap 4. setMaterial
class TerrainObject :
	public VBOObject
{
public:
	// constructor
	// stride defines the distance between two neighbour key dots.
	TerrainObject(int width, int height, float stride);
	// destructor
	~TerrainObject(void);

	// return a list of VBOMeshes that this VBOObject represent
	// this function return a pointer to the first VBOMesh of the 
	// list and num will be changed to the size of the list
	VBOMesh* representInVBOMesh(int* num);

	//return the type of this object
	int getType();

	//return the id of this object
	int getID();

	// set heightMap and calculate position, normal and texture
	// coordinate for this terrain.
	// height's length should be bigger than width*height.
	void setHeightMap(float* heights);

	// set material of this terrain, type is an id number of the
	// material defined in this class to use
	// type can be METAL_TERRAIN,SOIL_TERRAIN etc.
	void setMaterail(int type);

	// set function of startX and startY
	void setStartPosition(float x, float y);

	// get the height value of the terrain at given position
	// that position should be inside the terrain, if not, error
	// will be reported
	float getHeightAt(float x, float y);

	// check whether the given position is inside the area of this
	// terrain
	bool isInsideThisTerrain(float x, float y);

	// set function of texId
	void setTexId(GLuint texid);

private:

	// calculate position for the terrain, create the vbomesh and
	// set the position into it
	void calculatePosition();
	// calculate normals for the terrain, and set them to vbomesh
	void calculateNormal();
	// calculate texture coordinates for the terrain, and set them 
	// to vbomesh
	void calculateTextureCoordinate();

	// vbomesh that represent the terrain
	VBOMesh* vbomesh;

	// the width of this terrain
	int width;
	// the height of this terrain
	int height;
	// stride of this terrain
	float stride;
	// an array that record all the height information row by
	// row from left to right
	float* heightMap;

	// this terrain is a terrain vertical to +z
	// its height is in +z
	// startX, startY is the first height dot's x and y position
	float startX;
	float startY;

	// materials: the i-th 4 element of these arrays is the 
	// property of i-th type material
	GLfloat ambient[TERRAIN_MATERIAL_TYPE_NUM][4];
	GLfloat diffuse[TERRAIN_MATERIAL_TYPE_NUM][4];
	GLfloat specular[TERRAIN_MATERIAL_TYPE_NUM][4];
	GLfloat emission[TERRAIN_MATERIAL_TYPE_NUM][4];
	GLfloat shininess[TERRAIN_MATERIAL_TYPE_NUM];
	// texture id of the texture for this terrain
	GLuint texId;
};
