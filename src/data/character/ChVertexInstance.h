#pragma once

// ChVertexInstance is a class that stores the current position
// and normal of a vertex
// it has an id which equals to the ChVertex it corresponds to.
// this instance will be changed every frame according to the
// move of ChBoneInstance.
class ChVertexInstance
{
public:
	// constructor
	ChVertexInstance(int id);
	// destructor
	~ChVertexInstance(void);

	// get function of id
	int getId();

	// set function of position, p should be an array of 3
	void setPosition(float* p);

	// get function of position
	float* getPosition();

	// set function of normal, n should be an array of 3
	void setNormal(float* n);

	// get function of normal
	float* getNormal();

	// set function of hasNormal
	void setHasNormal(bool hn);

	// get function of hasNormal
	bool getHasNormal();

private:
	// 
	int id;
	// current position
	float position[3];
	// whether this vertex has normal, should equals to the ChVertex
	// it corresponds to
	bool hasNormal;
	// current normal;
	float normal[3];
};
