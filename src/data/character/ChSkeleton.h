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
	
	// calculate absolute transform for each bone
	void calculateState();

	// get bone by name
	// @param name the name for the bone
	// @return the pointer for the bone with the name
	//         if no bone has the name, create a new bone
	ChBone* getBone(const std::string &name);

	ChBone* getRootBone();

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
	// map bone name with bone pointer
	std::map< std::string, ChBone* > m_boneMap;
	// root bone pointer
	ChBone * m_rootBone;

};
