/* 
   BVHTree.h (Bounding Volume Hierarchy Tree)
   Katie Keim
   Deer - CPE 476
*/

#ifndef BVHTREE_H_
#define BVHTREE_H_

#include "bounding_rectangle.h"
#include "GameObject.h"

struct BVHTree {
   void calculateTree(const std::vector<GameObject*>& objects);
   std::vector<GameObject*> getCollidingObjects(const BoundingRectangle& bRect);

  private:
   std::vector<GameObject*> allNodes;
};

#endif //BVHTREE_H_
