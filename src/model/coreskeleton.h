#ifndef CHARACTER_CORE_SKELETON
#define CHARACTER_CORE_SKELETON

#include "global.h"
#include <map>

class CoreBone;

class CoreSkeleton{

public:
  ~CoreSkeleton();

  /*
   * create a new corebone,add it to the bonelist
   * 
   * @return the pointer to the new corebone
   *		 0 if failed to create a new corebone
   */
  CoreBone * newCoreBone(const std::string name);

  /**
   * Calculates the current state.
   *
   * This function calculates the current state (absolute translation and
   * rotation, as well as the bone space transformation) of the bone instance
   * and all its children.
   **/
  void calculateState();

  CoreBone* getCoreBone(int coreBoneId) const;
  CoreBone* getCoreBone(const std::string& name) const;
  int getCoreBoneId(const std::string& strName) const;

  std::vector<int>& getRootCoreBoneIds() const;
  std::vector<CoreBone *>& getCoreBones() const;

  bool contains(const std::string& name) const;

  void scale(float factor);

private:

  // the core bones that attached to the core skeleton
  std::vector<CoreBone *> m_coreBones;
  // the map between corebone name and id
  std::map< std::string, int > m_coreBoneNamesMap;
  // the ids of root core bones (bones that has no prarent)
  std::vector<int> m_rootCoreBoneIdList;
};

#endif