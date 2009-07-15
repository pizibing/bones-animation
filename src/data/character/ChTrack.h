#pragma once

#ifndef Matrix
#define Matrix matrix44
#endif

class Matrix;
class ChBone;
class ChKeyFrame;

// a animation controller for a bone
// has key frames to generate current frame
// must set key frame number and bone pointer when create a new track

class ChTrack
{
public:
	// constructor
	ChTrack(void);
	// destructor
	~ChTrack(void);

	// @param animate_time
	// @return the relative transform matrix for the bone
	const Matrix& getTransformMatrix(int animate_time);

	// @return the relative transform matrix of the last frame
	const Matrix& getLastTransformMatrix();

	// @return the bone of the track
	ChBone * getBone()const;
	
	// set bone of the track
	void setBone(ChBone * bone);

	// add a new key frame
	// @param matrix the transform matrix for the frame
	// @param frame_time the start time(sec) of the frame
	bool addKeyFrame(const Matrix &matrix, int frame_time);

	// @return the total animation time of the track
	int getAnimationTime();

	// initialized the key frame array
	// @param frame_num the array size
	// @return true if successful
	bool init(int frame_num);

private:
	int m_current_keyframe;
	// the total number of key frame
	int m_keyframe_num;
	// array for key frame pointers
	ChKeyFrame **m_keyframes;
	// bone to animate
	ChBone * m_bone;
	// current transform matrix
	Matrix m_currentMatrix;
};
