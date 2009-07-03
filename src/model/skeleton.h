#ifndef CHARACTER_SKELETON
#define CHARACTER_SKELETON

#include "global.h"

class CoreSkeleton;
class Bone;

class Skeleton
{
public:
  Skeleton(CoreSkeleton* pCoreSkeleton);
  ~Skeleton();

  void calculateState();
  void clearState();
  void lockState();

  Bone *getBone(int boneId) const;
  CoreSkeleton *getCoreSkeleton() const;
  std::vector<Bone *>& getBones() const;
  
  // DEBUG-CODE
  /*
   * get points that presents the skeleton,put them at pPoints
   * @return the total number of points
   */
  int getBonePoints(float *pPoints) const;
  /*
   * get lines that presents the skeleton,put them at pLines
   * @return the total number of points
   */
  int getBoneLines(float *pLines) const;

private:
  CoreSkeleton *m_coreSkeleton;
  std::vector<Bone *> m_bones;
};

#endif