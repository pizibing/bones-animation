#pragma once

#include <string>

class Matrix;

// bone for a character,such as head,hand,foot etc.
// has a parent bone pointer and a array for child bone pointers
// has a string name as id
// must set child number when create a new bone

class ChBone
{
public:
	// constructor
	// @param name the name for the bone
	ChBone(const std::string &name);
	// destructor
	~ChBone(void);

	const std::string& getName()const;

	ChBone * getParentBone()const;
	void SetParentBone(ChBone * parent);

	// add a child bone 
	// @return false if no room for new child bone
	bool addChildBone(ChBone *bone);

	// get the original relative transform matrix
	const Matrix& getOriginalTransformMatrix()const;

	void setOriginalTransformMatrix(const Matrix &m);

	// initialize this bone with child_num child bones
	// @param child_num child bone number
	// @return true if successful
	bool init(int child_num);

private:
	// string name for the bone
	std::string m_name;
	// child bone number
	int m_child_num;
	// parent bone pointer
	ChBone * m_parentbone;
	// child bone pointers
	ChBone ** m_childbones;
	
	// relative transformation to form original skeleton
	Matrix m_originalTransformMatrix;

};
