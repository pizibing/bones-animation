#ifndef CHARACTER_BONE_H
#define CHARACTER_BONE_H

#include "global.h"

class Skeleton;
class CoreBone;

class Bone{

public:
  
  Bone(CoreBone* coreBone);
  ~Bone() { }


  /**
   * Interpolates the current state to another state.
   *
   * This function interpolates the current state (relative translation and
   * rotation) of the bone instance to another state of a given weight.
   *
   * @param weight The blending weight.
   * @param translation The relative translation to be interpolated to.
   * @param rotation The relative rotation to be interpolated to.
   **/
  void blendState(float weight, const Vector3D& translation, const Quaternion& rotation);
  
  /**
   * Calculates the current state.
   *
   * This function calculates the current state (absolute translation and
   * rotation, as well as the bone space transformation) of the bone instance
   * and all its children.
   **/
  void calculateState();

   /**
   * Resets the bone to its core state
   *
   * This function changes the state of the bone to its default non-animated
   * position and orientation. Child bones are unaffected and may be animated
   * independently. 
   **/
  void setCoreState();

  /**
   * Resets the bone and children to core states
   *
   * This function changes the state of the bone to its default non-animated
   * position and orientation. All child bones are also set in this manner.
   **/
  void setCoreStateRecursive();

  CoreBone *getCoreBone() const;
  const Vector3D& getTranslation() const;
  const Quaternion& getRotation()const;
  const Vector3D& getAbsoluteTranslation() const;
  const Quaternion& getAbsoluteRotation()const;
  const Vector3D& getBoneSpaceTranslation() const;
  const Quaternion& getBoneSpaceRotation() const;
  
  void setSkeleton(Skeleton *skeleton);
  void setParentId(int parentId);

  void setTranslation(const Vector3D& translation);
  void setRotation(const Quaternion& rotation);

  void scale(float factor);

  const CharacterMatrix& getTransformMatrix();
  
  
private:
  // the name for the bone
  std::string m_name; 
  // pointer to the skeleton which the bone is attached
  Skeleton *m_skeleton;
  // the id in skeleton for the parrent bone
  int m_parentId;
  // the id list for child bones
  std::vector<int> m_childIds;
  
  // the relative translation
  Vector3D m_translation;
  // the relative rotation
  Quaternion m_rotation;
  
  // the absolute translation
  Vector3D m_absoluteTranslation;
  // the absolute rotation
  Quaternion m_absoluteRotation;

  // the translation in bone space
  Vector3D m_boneSpaceTranslation;
  // the rotation in bone space
  Quaternion m_boneSpaceRotation;

  // transform matrix for opengl 
  CharacterMatrix m_transformMatrix;  

};

#endif