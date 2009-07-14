#pragma once

class matrix44;

// vertex and bone pair
// it tells which bone is related to, transforming matrix of 
// this bone and the power of this bone
struct VBpair{
    // the related bone's id
	int boneId;
	// the relative position of this vertex to the bone
	matrix44 matrix;
	// this bone's power to the vertex
	float power;
};

// ChVertex stores the related information of a vertex
// i.e which bones are this vertex related to, transforming
// matrix of each bone and the power of each bone
class ChVertex
{
public:
	// constructor
	// initialize the default position by the given px,py,pz
	// set default id to -1
	ChVertex(float px, float py, float pz);
	// destructor
	~ChVertex(void);

	// set the default normal of the vertex
	// hasNormal will be set to true and normal[3] will be set
	// to the given value
	void setDefaultNormal(float nx, float ny, float nz);

	// initialize the pairs 
	// if pairs has never been initialized, a new array will be 
	// created whose length will be num
	// else nothing happens
	void initPairs(int num);

	// initialize the pairNum-th VBpair of pairs
	// matrix should be a float[16]
	// boneId, matrix, power will be set to the corresponding properties
	// in VBpair
	void initPair(int pairNum, int boneId, float* matrix, float power);

	// set id of this vertex to id
	void setId(int id);

	// get the id of this vertex, if id has not been set , the default 
	// id is -1
	int getId();

	// get function of position
	float* getPosition();

	// get function of normal
	float* getNormal();

	// get function of hasNormal
	bool getHasNormal();

	// get function of pairs
	VBpair* getPairs();

	// get function of pairNum
	int getPairNum();

private:
	// a pointer to an array that stores all the VBpairs of a
	// vertex
	VBpair* pairs;
	// length of pairs
	int pairNum;
	// default position of the vertex to the root of the 
	// CharacterObject
	float position[3];
	// whether this vertex has normal
	bool hasNormal;
	// default normal of the vertex in world space
	float normal[3];
	// id of this vertex
	int id;
};
