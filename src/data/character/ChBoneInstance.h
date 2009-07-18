#pragma once

class Matrix;

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

	// set function of rotation
	void setRotation(const Quaternion &q);

	// get function of rotation
	const Quaternion & getRotation();

	// set function of translation
	void setTranslation(const Vector3D &v);

	// get function of translation
	const Vector3D & getTranslation();

	// set function of absolute rotation
	void setAbsoluteRotation(const Quaternion &q);

	// get function of absolute rotation
	const Quaternion & getAbsoluteRotation();

	// set function of absolute translation
	void setAbsoluteTranslation(const Vector3D &v);

	// get function of absolute translation
	const Vector3D & getAbsoluteTranslation();

	// set function of matrix
	void setMatrix(const Matrix &matrix);

	// get function of matrix
	const Matrix & getMatrix();

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

	// get function of inverse
	Matrix getInverse();

	// set function of inverse
	void setInverse(Matrix inverse);

private:
	// the id of this ChBoneInstance 
	// it should equals to the corresponding ChBone's id
	int id;
	// a matrix describes the current position of this bone instance
	// the position is its relative position to the root
	Matrix matrix;
	// rotation in local space
	Quaternion rotation;
	// translation in world space
	Vector3D  translation;
	// rotation in local space
	Quaternion absoluteRotation;
	// translation in world space
	Vector3D  absoluteTranslation;
	// the inverse matrix of the default absolute transform matrix of this bone
	Matrix inverse;

	// the id of this bone instance's father, root's fatherId is set
	// to -1
	int fatherId;
	// an array of all the children's id
	int* childId;
	// childId's length
	int childCount;
};
