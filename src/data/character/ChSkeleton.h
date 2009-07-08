#pragma once

// Skeleton for a character
// contains the pointers of all bones
// must set bone total number when create a new skeleton

#include <map>

class ChBone;

class ChSkeleton
{
public:
	// constructor
	ChSkeleton(void);
	// destructor
	~ChSkeleton(void);
	

private:
	ChBone (*m_bones)[];
	std::map< char*, int > m_boneNameMap;
	ChBone * m_rootBone;

};
