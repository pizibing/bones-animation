#pragma once

class Matrix;
class Quaternion;
class Vector3D;

// key frame for a bone animation
// contains a 4*4 matrix for bone relative transform
// and a 4*4 matrix for bone absolute transform

class ChKeyFrame
{
public:
	// default constructor
	ChKeyFrame(void):m_time(0){};

	// constructor
	// @param matrix the transform matrix
	// @param frame_time the start time(sec) of the frame
	ChKeyFrame(const Matrix &matrix, int frame_time);

	// destructor
	inline ~ChKeyFrame(void){}

	// @return the transform matrix
	//inline const Matrix & getTransformMatrix()const{
	//	return m_transformMatrix;
	//}
	
	// @return a quaternion for rotation of the frame
	const Quaternion &getRotation() const{
		return m_rotation;
	}

	// @return a vector3d for translation of the frame
	const Vector3D &getTranslation() const{
		return m_translation;
	}

	// set the transform matrix
	void setTransformMatrix(const Matrix & matrix);

	// set the rotation quaternion
	void setRotation(const Quaternion & rotation);

	// set the translation vector3d
	void setTranslation(const Vector3D & translation);

	// @return the start time of the frame
	inline int getTime(){
		return m_time;
	}

	// set the start time of the frame
	inline void setTime(int time){
		m_time = time;
	}

private:
	// relative transform matrix in parent bone space
	//Matrix m_transformMatrix;
	// quaternion for rotation
	Quaternion m_rotation;
	// vector3 for translation
	Vector3D m_translation;

	// the start time of the frame
	// the frame order number of the key frame
	int m_time;

};
