#pragma once

class Matrix;
class Quaternion;
class Vector3D;
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
	//const Matrix& getTransformMatrix(int animate_time);
	
	// @param animate_time the frame time
	// @return the relative rotate transform for the bone
	const Quaternion & getRotation(int animate_time);

	// @param animate_time the frame time
	// @return the relative translate transform for the bone
	const Vector3D & getTranslation(int animate_time);

	// @return the relative transform matrix of the last frame
	//const Matrix& getLastTransformMatrix();

	// @return the relative rotate transform of the last frame
	const Quaternion& getLastRotation() const;

	// @return the relative translate transform of the last frame
	const Vector3D& getLastTranslation() const;

	// @return the bone of the track
	ChBone * getBone()const;
	
	// set bone of the track
	void setBone(ChBone * bone);

	// add a new key frame
	// @param matrix the transform matrix for the frame
	// @param frame_time the start time(sec) of the frame
	bool addKeyFrame(const Matrix &matrix, int frame_time);

	// @return the total animation time of the track
	int getAnimationTime() const;

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
	//Matrix m_currentMatrix;
	// quaternion for temp
	Quaternion m_currentRotation;
	// vector3d for temp
	Vector3D m_currentTranslation;
};
