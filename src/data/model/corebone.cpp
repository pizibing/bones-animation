#include "global.h"
#include "CoreBone.h"
#include "coreskeleton.h"

  CoreBone::CoreBone(const std::string& name)
	  : m_name(name),
	    m_parentId(-1),
		m_coreSkeleton(NULL){
  }

  bool CoreBone::addChildId(int childId){
	  m_childIds.push_back(childId);
	  return true;
  }

  /**
   * Calculates the current state.
   *
   * This function calculates the current state (absolute translation and
   * rotation, as well as the bone space transformation) of the bone instance
   * and all its children.
   **/
  void CoreBone::calculateState(){
	  
	  //if( m_parentId == -1 ){
		 // // this is a root corebone
		 // m_absoluteTranslation = m_translation;
		 // m_absoluteRotation = m_rotation;
	  //}
	  //else {
		 // CoreBone * parent = m_coreSkeleton->getCoreBone(m_parentId);

		 // m_absoluteTranslation = m_translation;
		 // m_absoluteTranslation += parent->getAbsoluteRotation();
		 // m_absoluteTranslation *= parent->getAbsoluteTranslation();

		 // m_absoluteRotation = m_rotation;
		 // m_absoluteRotation *= parent->getAbsoluteRotation();
	  //}

	  //// calculate all child bones
	  //std::vector<int>::iterator childIterator;
	  //for(childIterator = m_childIds.begin();childIterator!=m_childIds.end();childIterator++){
		 // m_coreSkeleton->getCoreBone(*childIterator)->calculateState();
	  //}
  }

  std::vector<int>& CoreBone::getChildIds(){
	  return m_childIds;
  }
  const std::string& CoreBone::getName() const{
	  return m_name;
  }
  int CoreBone::getParentId() const{
	  return m_parentId;
  }
  CoreSkeleton * CoreBone::getCoreSkeleton() const{
	  return m_coreSkeleton;

  }

  const Vector3D& CoreBone::getTranslation() const{
	  return m_translation;
  }

  const Quaternion& CoreBone::getRotation() const{
	  return m_rotation;
  }

  const Vector3D& CoreBone::getAbsoluteTranslation() const{
	  return m_absoluteTranslation;
  }

  const Quaternion& CoreBone::getAbsoluteRotation() const{
	  return m_absoluteRotation;
  }

  const Vector3D& CoreBone::getBoneSpaceTranslation() const{
	  return m_boneSpaceTranslation;
  }

  const Quaternion& CoreBone::getBoneSpaceRotation() const{
	  return m_boneSpaceRotation;
  }
  
  void CoreBone::setCoreSkeleton(CoreSkeleton *coreSkeleton){
	  m_coreSkeleton = coreSkeleton;
  }

  void CoreBone::setParentId(int parentId){
	  m_parentId = parentId;
  }
  
  void CoreBone::setRotation(const Quaternion& rotation){
	  m_rotation = rotation;
  }

  void CoreBone::setBoneSpaceRotation(const Quaternion& rotation){
	  m_boneSpaceRotation = rotation;
  }

  void CoreBone::setTranslation(const Vector3D& translation){
	  m_translation = translation;
  }

  void CoreBone::setBoneSpaceTranslation(const Vector3D& translation){
	  m_boneSpaceTranslation = translation;
  }

  void CoreBone::scale(float factor){
	  m_translation*=factor;
	  m_absoluteTranslation*=factor;
	  m_boneSpaceTranslation*=factor;
	
	  // factor all child bones
	  std::vector<int>::iterator iteratorChildId;
	  for(iteratorChildId = m_childIds.begin(); iteratorChildId != m_childIds.end(); ++iteratorChildId){
		  m_coreSkeleton->getCoreBone(*iteratorChildId)->scale(factor);
	  }
  }