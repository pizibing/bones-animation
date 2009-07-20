#pragma once

class ChVertex;

// ChSkin stores all the vertices(ChVertex) that the 
// CharacterObject refer to it contains.
class ChSkin
{
public:
	// constructor
	ChSkin(ChSkeleton* skeleton);
	// destructor
	~ChSkin(void);

	// initialize vertices and verticeSize
	// if vertices has never been initialized, a new array
	// of ChVertex will be created whose length is num
	// else nothing will happen
	void initVertices(int num);

	// set the verNum-th ChVertex of vertices to vertex
	// verNum should be smaller than num
	// vertex's id will be set to verNum
	ChVertex* initVertice(int verNum);

	// get function of vertices
	ChVertex** getVertices();

	// get function of verticeSize
	int getVerticeSize();

	// return the vertex with the given id
	ChVertex* getVertex(int id);

private:
	// a pointer to an array that stores all the vertices
	ChVertex** vertices;
	// length of vertices
	int verticeSize;
	// the skeleton the skin is on
	ChSkeleton* skeleton;
};
