#ifndef CHARACTER_CORESKELETON_H
#define CHARACTER_CORESKELETON_H

#include <string>
#include <vector>
#include <map>

class Vector3D;
class Quaternion;
class CoreBone;

class CoreSkeleton{

public:
  CoreSkeleton();
  ~CoreSkeleton();

  /*
   * create a new CoreBone,add it to the bone vector
   * 
   * @return the pointer to the new CoreBone
   *		 0 if failed to create a new CoreBone
   */
  CoreBone * newCoreBone(const std::string name, bool root);

  /**
   * Calculates the current state.
   *
   * This function calculates the current state (absolute translation and
   * rotation, as well as the bone space transformation) of the bone instance
   * and all its children.
   **/
  void calculateState();

  CoreBone* getCoreBone(int CoreBoneId);
  CoreBone* getCoreBone(const std::string& name);
  int getCoreBoneId(const std::string& strName);

  std::vector<CoreBone *>& getRootCoreBones();
  std::vector<CoreBone *>& getCoreBones();

  void scale(float factor);

private:

  // the core bones that attached to the core skeleton
  std::vector<CoreBone *> m_coreBones;
  // the map between CoreBone name and id
  std::map< std::string, int > m_coreBoneNamesMap;
  // the  root core bones (bones that has no parent)
  std::vector<CoreBone *> m_rootCoreBones;

};

#endif
