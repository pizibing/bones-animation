#pragma once

class matrix44;

// ChBoneInstance is a class that stores the current instance
// of a character's bone.
// this instance will be changed every frame according to the
// animation.
class ChBoneInstance
{
public:
	// constructor
	// initialize id to id
	ChBoneInstance(int id);
	// destructor
	~ChBoneInstance(void);

	// set function of matrix
	void setMatrix(matrix44* matrix);

	// get function of matrix
	matrix44* getMatrix();

	// get function of id
	int getId();

	// set function of fatherId
	void setFatherId(int id);

	// get function of fatherId ,root's fatherId is -1
	int getFatherId();

	// initialize the childId array, set the length to num
	// if childId has been initialized, do nothing
	void initChildId(int num);

	// set the num-th childId to id
	void setChildId(int num, int id);

	// get function of childId
	// return the pointer to the first element of childId, num will 
	// be set to the length of childId
	// if this bone instance is the leaf, NULL will be return, num
	// will be set to 0
	int* getChildId(int* num);

private:
	// the id of this ChBoneInstance 
	// it should equals to the corresponding ChBone's id
	int id;
	// a matrix describes the current position of this bone instance
	// the position is its relative position to the root
	matrix44* matrix;
	// the id of this bone instance's father, root's fatherId is set
	// to -1
	int fatherId;
	// an array of all the children's id
	int* childId;
	// childId's length
	int childCount;
};
