#pragma once

#include <string>

class Vector3D;
class Quaternion;
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

	// the name for the bone
	const std::string& getName()const;
	
	// @return the index in skeleton
	int getId() const;
	// set the index in skeleton
	void setId(int id);

	// @return the pointer of the parent bone
	ChBone * getParentBone()const;
	
	// set the parent bone
	void SetParentBone(ChBone * parent);

	// get the array of child bone pointers
	ChBone** getAllChildBones()const;

	// @return the number of child bones
	int getChildBoneNum();

	// add a child bone 
	// @return false if no room for new child bone
	bool addChildBone(ChBone *bone);

	// get the original relative transform matrix
	//const Matrix& getTransformMatrix()const;

	// get the rotation transform in local space
	const Quaternion& getRotation() const;

	// get the translation transform in local space
	const Vector3D& getTranslation() const;

	// get the rotation transform in world space
	const Quaternion& getAbsoluteRotation() const;

	// get the translation transform in world space
	const Vector3D& getAbsoluteTranslation() const;

	// set the relative transform matrix
	void setTransformMatrix(const Matrix &m);

	// set the absolute transform matrix
	void setAbsolteTransformMatrix(const Matrix &m);

	// get the bind pose inverse matrix
	const Matrix & getBindPoseInverse();
	// set the bind pose inverse matrix
	void setBindPoseInverse(const Matrix & m);

	// initialize this bone with child_num child bones
	// @param child_num child bone number
	// @return true if successful
	//		   false if reinitialized or no more memory
	bool init(int child_num);

private:
	// index in skeleton
	int m_id;
	// string name for the bone
	std::string m_name;
	// child bone number
	int m_child_num;
	// parent bone pointer
	ChBone * m_parentbone;
	// child bone pointers
	ChBone ** m_childbones;

	// current child bone number
	int m_current_child_num;
	
	// relative transformation to form original skeleton
	//Matrix m_originalTransformMatrix;
	
	// rotation in local space
	Quaternion m_rotation;
	// translation in local space
	Vector3D m_translation;
	// rotation in world space
	Quaternion m_absoluteRotation;
	// translation in world space
	Vector3D m_absoluteTranslation;

	Matrix 	m_bindPoseInverse;

};
