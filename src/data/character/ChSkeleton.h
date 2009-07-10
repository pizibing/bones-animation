#pragma once

#include <string>
#include <map>

class ChBone;

// Skeleton for a character
// contains the pointers of all bones
// must set bone total number when create a new skeleton
class ChSkeleton
{
public:
	// constructor
	ChSkeleton(void);
	// destructor
	~ChSkeleton(void);

	// get bone by name
	// @param name the name for the bone
	// @return the pointer for the bone with the name
	//         if no bone has the name, create a new bone
	ChBone* getBone(const std::string &name)const;

	// get bone by index
	// @param boneId the index for the bone
	// @return the pointer for the bone with the index
	//         if no bone has the index return NULL
	ChBone* getBone(int boneId);

	// get the index of bone with the name
	// @param name the name for the bone
	// @return the index of the bone 
	int getBoneId(const std::string &name);

	// get the array of all bones
	ChBone** getAllBones()const;
	
	// @return the number of bones
	int getBoneNum() const;

	// @return the root bone pointer
	ChBone* getRootBone()const;

	// set root bone
	void setRootBone(ChBone *bone);

	// init the pointer array for bones
	// skeleton can be initialized only once
	// @param bone_num the number of bones
	// @return true if successful 
	bool init(int bone_num);

private:
	// the bone name
	int m_bone_num;
	// bone pointers
	ChBone **m_bones;
	// map bone name with bone index
	std::map< std::string, int > m_boneMap;
	// root bone pointer
	ChBone * m_rootBone;

};
