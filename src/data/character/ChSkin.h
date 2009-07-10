#pragma once

class ChVertex;

// ChSkin stores all the vertices(ChVertex) that the 
// CharacterObject refer to it contains.
class ChSkin
{
public:
	// constructor
	ChSkin(void);
	// destructor
	~ChSkin(void);

	// initialize vertices
	// if vertices has never been initialized, a new array
	// of ChVertex will be created whose length is num
	// else nothing will happen
	void initVertices(int num);

	// set the verNum-th ChVertex of vertices to vertex
	// verNum should be smaller than num
	// vertex's id will be set to verNum
	void initVertice(int verNum, ChVertex* vertex);

private:
	// a pointer to an array that stores all the vertices
	ChVertex** vertices;
};
