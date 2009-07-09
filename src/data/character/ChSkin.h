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

private:
	// a pointer to an array that stores all the vertices
	ChVertex* vertices;
};
