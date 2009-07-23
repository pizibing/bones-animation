#pragma once

#include <string>

class Vector3D;
class Quaternion;
class Matrix;
class ChBone;
class ChSkeleton;
class ChTrack;

// animation for a character,such as walk,run etc.
// must set skeleton bone number when create a new Animation
// has the same number of tracks with skeleton's bones

class ChAnimation
{
public:
	// constructor
	ChAnimation();

	// destructor
	~ChAnimation(void);

	// @return the name for the animation
	const std::string& getName();

	// set the name of the animation
	void setName(const std::string &name);

	// set skeleton and init the animation
	// and initialize track of bone
	void setSkeleton(ChSkeleton *skeleton);

	// @return the track with the bonename
	ChTrack * getTrack(std::string bonename);

	// @return the track with the boneId
	ChTrack * getTrack(int boneId);

	// calculate the transform matrix for the bone
	// @param animatetime the time in second from the beginning of the
	//        animation
	// @param boneId the index for the blending bone
	// @return the transform matrix for the bone
	//const Matrix& blendBone(int animatetime,int boneId);

	// calculate the rotate transform for the bone
	// @param animatetime the time in second from the beginning of the
	//        animation
	// @param boneId the index for the blending bone
	// @return the rotate transform for the bone
	const Quaternion& blendBoneRotation(int animatetime,int boneId);

	// calculate the translate transform for the bone
	// @param animatetime the time in second from the beginning of the
	//        animation
	// @param boneId the index for the blending bone
	// @return the translate transform for the bone
	const Vector3D& blendBoneTranslation(int animatetime,int boneId);


	// @return the total time of the animation;
	int getAnimationTime();

private:
	// initialize the track array
	bool init(int track_num);

private:
	std::string m_name;
	//int m_frame_time;
	// total animation time
	int m_key_frame_num;
	// number of track, the same with skeleton bone number
	int m_track_num;
	// skeleton to animate
	ChSkeleton *m_skeleton;

	// track pointer array
	ChTrack **m_tracks; 

};
