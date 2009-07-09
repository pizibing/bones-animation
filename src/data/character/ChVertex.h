#pragma once

class ChBone;

// vertex and bone pair
// it tells which bone is related to, transforming matrix of 
// this bone and the power of this bone
struct VBpair{
	ChBone* bone;
	float matrix[16];
	float power;
};

// ChVertex stores the related information of a vertex
// i.e which bones are this vertex related to, transforming
// matrix of each bone and the power of each bone
class ChVertex
{
public:
	// constructor
	ChVertex(void);
	// destructor
	~ChVertex(void);

private:
	// a pointer to an array that stores all the VBpairs of a
	// vertex
	VBpair* pairs;
};
