#ifndef CHARACTER_CORE_BONE
#define CHARACTER_CORE_BONE

#include "global.h"

class CoreSkeleton;

class Corbone{

public:
  Corbone(const std::string& name);
  ~Corbone() { }

  bool addChildId(int childId);

  /**
   * Calculates the current state.
   *
   * This function calculates the current state (absolute translation and
   * rotation, as well as the bone space transformation) of the bone instance
   * and all its children.
   **/
  void calculateState();

  std::vector<int>& getChildIdList() const;
  const std::string& getName() const;
  int getParentId() const;
  CoreSkeleton *getCoreSkeleton() const;

  const Quaternion& getRotation() const;
  const Quaternion& getAbsoluteRotation() const;
  const Quaternion& getBoneSpaceRotation() const;
  const Vector3D& getTranslation() const;
  const Vector3D& getAbsoluteTranslation() const;
  const Vector3D& getBoneSpaceTranslation() const;
  
  void setCoreSkeleton(CoreSkeleton *skeleton);
  void setParentId(int parentId);
  
  void setRotation(const Quaternion& rotation);
  void setBoneSpaceRotation(const Quaternion& rotation);
  void setTranslation(const Vector3D& translation);
  void setBoneSpaceTranslation(const Vector3D& translation);

  void scale(float factor);

private:
  // the name for the bone
  std::string m_name;
  // pointer to the coreskeleton which the corebone is attached
  CoreSkeleton *m_coreSkeleton;
  // the id in skeleton for the parrent bone
  int m_parentId;
  // the id list for child bones
  std::vector<int> m_childIdList;
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

};

#endif