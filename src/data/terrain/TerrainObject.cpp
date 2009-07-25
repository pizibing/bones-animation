#include <assert.h>
#include "../matrixlib/Vector3D.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "TerrainObject.h"
#include "../managers/ObjectManager.h"

// constructor
// stride defines the distance between two neighbour key dots.
TerrainObject::TerrainObject(int width, int height, float stride){

	// initialize vbomesh to be an empty vbomesh
	vbomesh = new VBOMesh();

	// type is OBJECT_TYPE_TERRAIN
	type = OBJECT_TYPE_TERRAIN;

	// generate id
	ObjectManager* om = ObjectManager::getInstance();
	id = om->generateID();

	// set width and height
	this->width = width;
	this->height = height;
	this->stride = stride;
	// initialize heightMap
	heightMap = new float[width * height];

	// initialize material
	// initialize texId
	texId = -1;

	// initialize start position
	startX = -width / 2 * stride;
	startY = -height / 2 * stride;
}

// destructor
TerrainObject::~TerrainObject(void){
	if(vbomesh) delete vbomesh;

	if(heightMap) delete[] heightMap;
	heightMap = 0;
}

// return a list of VBOMeshes that this VBOObject represent
// this function return a pointer to the first VBOMesh of the 
// list and num will be changed to the size of the list
VBOMesh* TerrainObject::representInVBOMesh(int* num){
	// the number of vbomeshes that represent terrain is 1
	*num = 1;

	return vbomesh;
}

//return the type of this object
int TerrainObject::getType(){
	return type;
}

//return the id of this object
int TerrainObject::getID(){
	return id;
}

// set heightMap and calculate position, normal and texture
// coordinate for this terrain.
// height's length should be bigger than width*height.
void TerrainObject::setHeightMap(float* heights){
	// copy height data into heightMap
	for(int i = 0; i < width * height; i++){
		// height's i-th element should not be null
		assert(heights + i);

		// copy
		heightMap[i] = heights[i];
	}

	// calculate position, normal and texture
	calculatePosition();
	calculateNormal();
	calculateTextureCoordinate();
}

// set material of this terrain, type is an id number of the
// material defined in this class to use
// type can be METAL_TERRAIN,SOIL_TERRAIN etc.
void TerrainObject::setMaterail(int type){
	// set material
	vbomesh->setMaterial(ambient[type],diffuse[type],specular[type],
		                 emission[type],shininess[type]);
}

// calculate position for the terrain, create the vbomesh and
// set the position into it
void TerrainObject::calculatePosition(){
	/* generate the vertices array according to heightMap, stride,
       startX, startY, width and height */
	// calculate size of vertices (vbomesh's vertices)
	int size = (width - 1) * (height - 1) * 2 * 3 * 3;
	// create the vertices
	GLfloat* vertices = new GLfloat[size];
	/* set vertices' value */
	// pointer to the vertices element
	int i = 0;
	// for each row except the last
	for(int r = 0; r < height - 1; r++){
		// for each column except the last
		for(int c = 0; c < width - 1; c++){
			// get the 4 vertices of the quad whose upper and left
			// edge is row and column
			GLfloat ulh = heightMap[r * width + c];
			GLfloat urh = heightMap[r * width + c + 1];
			GLfloat blh = heightMap[(r + 1) * width + c];
			GLfloat brh = heightMap[(r + 1) * width + c + 1];
			// get x, y
			GLfloat lcx = startX + stride * c;
			GLfloat rcx = startX + stride * (c + 1);
			GLfloat ury = startY + stride * r;
			GLfloat bry = startY + stride * (r + 1);
			/* set vertices */
			/* first triangle */
			// first dot
			vertices[i] = lcx; i++;
			vertices[i] = ury; i++;
			vertices[i] = ulh; i++;
			// second dot
			vertices[i] = rcx; i++;
			vertices[i] = ury; i++;
			vertices[i] = urh; i++;
			// third dot
			vertices[i] = lcx; i++;
			vertices[i] = bry; i++;
			vertices[i] = blh; i++;
			/* second triangle */
			// first dot
			vertices[i] = rcx; i++;
			vertices[i] = ury; i++;
			vertices[i] = urh; i++;
			// second dot
			vertices[i] = lcx; i++;
			vertices[i] = bry; i++;
			vertices[i] = blh; i++;
			// third dot
			vertices[i] = rcx; i++;
			vertices[i] = bry; i++;
			vertices[i] = brh; i++;
		}
	}
	// set vertices into vbomesh, use GL_STATIC_DRAW_ARB for position
	// of this terrain will never change.
	vbomesh->setVertices(vertices,size,GL_STATIC_DRAW_ARB);

	// delete vertices
	delete[] vertices;
}

// calculate normals for the terrain, and set them to vbomesh
void TerrainObject::calculateNormal(){
	// create a normal list of every dot, its order is the same
	// as heightMap
	Vector3D* normals = new Vector3D[width * height];
	// initialize normals to 0 vector
	for(int i = 0; i < width * height; i++)
		normals[i].Set(0,0,0);

	/* for each triangle, calculate its normal and add this normal
	   to all vertices it relates */
	// for each row except the last
	for(int r = 0; r < height - 1; r++){
		// for each column except the last
		for(int c = 0; c < width - 1; c++){
			/* calculate normals of the 2 triangles constructed by
			 row and column */
			// first triangle
			Vector3D v1 = Vector3D(0,stride,
				heightMap[(r + 1) * width + c] - heightMap[r * width + c]);
			Vector3D v2 = Vector3D(stride,0,
				heightMap[r * width + c + 1] - heightMap[r * width + c]);
			Vector3D tv = v1 * v2;
			// add vector to related dots
			normals[r * width + c] += tv;
			normals[r * width + c + 1] += tv;
			normals[(r + 1) * width + c] += tv;
			// second triangle
			v1 = Vector3D(0,-stride,
				heightMap[r * width + c + 1] - heightMap[(r + 1) * width + c + 1]);
			v2 = Vector3D(-stride,0,
				heightMap[(r + 1) * width + c] - heightMap[(r + 1) * width + c + 1]);
			tv = v1 * v2;
			// add vector to related dots
			normals[r * width + c + 1] += tv;
			normals[(r + 1) * width + c] += tv;
			normals[(r + 1) * width + c + 1] += tv;
		}
	}
	/* normalize the normals */
	for(int i = 0; i < width * height; i++)
		normals[i].Unitize();
	/* build an array maps vbomesh dots to normals */
	int size = (width - 1) * (height - 1) * 2 * 3 * 3;
	GLfloat* normalsF = new GLfloat[size];
	/* initialize normalsF */
	// pointer to normalsF element
	int i = 0;
	// for each row except the last
	for(int r = 0; r < height - 1; r++){
		// for each column except the last
		for(int c = 0; c < width - 1; c++){
			// get normal of for related dot
			Vector3D uln = normals[r * width + c];
			Vector3D urn = normals[r * width + c + 1];
			Vector3D bln = normals[(r + 1) * width + c];
			Vector3D brn = normals[(r + 1) * width + c + 1];
			// set normalF
			// first triangle
			// first dot
			normalsF[i] = uln.x; i++;
			normalsF[i] = uln.y; i++;
			normalsF[i] = uln.z; i++;
			// second dot
			normalsF[i] = urn.x; i++;
			normalsF[i] = urn.y; i++;
			normalsF[i] = urn.z; i++;
			// third dot
			normalsF[i] = bln.x; i++;
			normalsF[i] = bln.y; i++;
			normalsF[i] = bln.z; i++;
			// second triangle
			// first dot
			normalsF[i] = urn.x; i++;
			normalsF[i] = urn.y; i++;
			normalsF[i] = urn.z; i++;
			// second dot
			normalsF[i] = bln.x; i++;
			normalsF[i] = bln.y; i++;
			normalsF[i] = bln.z; i++;
			// third dot
			normalsF[i] = brn.x; i++;
			normalsF[i] = brn.y; i++;
			normalsF[i] = brn.z; i++;
		}
	}
	/* set normal to vbomesh */
	vbomesh->setNormals(normalsF,size,GL_STATIC_DRAW_ARB);
	// delete
	delete[] normals;
	delete[] normalsF;
}

// calculate texture coordinates for the terrain, and set them 
// to vbomesh
void TerrainObject::calculateTextureCoordinate(){
	/* create an array in the same order as heightMap which records
	   the information of every dot's texture coordinate */
	GLfloat* textures = new GLfloat[height * width * 2];
	//init textures
	// for each row
	for(int r = 0; r < height; r++){
		// for each column
		for(int c = 0; c < width; c++){
			// set dot's texture coordinate
			// x
			textures[(r * width + c) * 2] = c / (width - 1);
			// y
			textures[(r * width + c) * 2 + 1] = r / (height - 1);
		}
	}
	/* convert textures into an array that map texture coordinates
	   to vbomesh */
	int size = (width - 1) * (height - 1) * 2 * 3 * 2;
	GLfloat* texturesF = new GLfloat[size];
	// set value of texturesF
	int i = 0;
	// for each row except the last
	for(int r = 0; r < height - 1; r++){
		// for each column except the last
		for(int c = 0; c < width - 1; c++){
			// first triangle
			// first dot
			texturesF[i] = textures[(r * width + c) * 2]; i++;
			texturesF[i] = textures[(r * width + c) * 2 + 1]; i++;
			// second dot
			texturesF[i] = textures[(r * width + c + 1) * 2]; i++;
			texturesF[i] = textures[(r * width + c + 1) * 2 + 1]; i++;
			// third dot
			texturesF[i] = textures[((r + 1) * width + c) * 2]; i++;
			texturesF[i] = textures[((r + 1) * width + c) * 2 + 1]; i++;
			// second triangle
			// first dot
			texturesF[i] = textures[(r * width + c + 1) * 2]; i++;
			texturesF[i] = textures[(r * width + c + 1) * 2 + 1]; i++;
			// second dot
			texturesF[i] = textures[((r + 1) * width + c) * 2]; i++;
			texturesF[i] = textures[((r + 1) * width + c) * 2 + 1]; i++;
			// third dot
			texturesF[i] = textures[((r + 1) * width + c + 1) * 2]; i++;
			texturesF[i] = textures[((r + 1) * width + c + 1) * 2 + 1]; i++;
		}
	}
	/* set texturesF to vbomesh */
	// texId should have been set before using this function
	assert(texId != -1);
	vbomesh->setTextures(texturesF,size,GL_STATIC_DRAW_ARB,texId);
	/* delete array created in this function */
	delete[] textures;
	delete[] texturesF;
}

// set function of startX and startY
void TerrainObject::setStartPosition(float x, float y){
	startX = x;
	startY = y;
}

// get the height value of the terrain at given position
float TerrainObject::getHeightAt(float x, float y){
	// x, y should be inside terrain
	assert(isInsideThisTerrain(x,y));
	/* decide which triangle this position is in */
	bool isTriangle1 = true;
	int r = (y - startY) / stride;
	int c = (x - startX) / stride;
	// calculate position's distance to each triangle
	float distance1 = (x - (startX + stride * c)) * (x - (startX + stride * c))
		            + (y - (startY + stride * r)) * (y - (startY + stride * r));
	float distance2 = (x - (startX + stride * (c + 1))) * (x - (startX + stride * (c + 1)))
		            + (y - (startY + stride * (r + 1))) * (y - (startY + stride * (r + 1)));
	if(distance1 > distance2) isTriangle1 = false;
	else isTriangle1 = true;
	// set three dot which decide the plain
	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
	x1 = (c + 1) * stride + startX;
	y1 = r * stride + startY;
	z1 = heightMap[r * width + c + 1];
	x2 = c * stride + startX;
	y2 = (r + 1) * stride + startY;
	z2 = heightMap[(r + 1) * width + c];
	if(isTriangle1){
		x3 = c * stride + startX;
		y3 = r * stride + startY;
		z3 = heightMap[r * width + c];
	}
	else{
		x3 = (c + 1) * stride + startX;
		y3 = (r + 1) * stride + startY;
		z3 = heightMap[(r + 1) * width + c + 1];
	}
	/* calculate the height via the plain forum */
	float u = ((x - x3) * (y2 - y3) - (x2 - x3) * (y - y3)) /
		      ((x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3));
	float v = ((x - x3) * (y1 - y3) - (x1 - x3) * (y - y3)) /
		      ((x2 - x3) * (y1 - y3) - (x1 - x3) * (y2 - y3));
	float resultH = u * (z1 - z3) + v * (z2 - z3) + z3;

	return resultH;
}

// check whether the given position is inside the area of this
// terrain
bool TerrainObject::isInsideThisTerrain(float x, float y){
	// check x
	if(x < startX) return false;
	if(x > startX + stride * (width - 1)) return false;
	// check y
	if(y < startY) return false;
	if(y > startY + stride * (height - 1)) return false;

	// all pass
	return true;
}

// set function of texId
void TerrainObject::setTexId(GLuint texid){
	texId = texid;
}
