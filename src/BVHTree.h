/*
BVHTree.h (Bounding Volume Hierarchy Tree)
Katie Keim
Deer - CPE 476
*/

#ifndef BVHTREE_H_
#define BVHTREE_H_

#include "bounding_rectangle.h"
#include "GameObject.h"
#include "BVHNode.h"
#include <list>

struct BVHTree {
   BVHTree();
   void calculateTree(std::vector<GameObject*> objects);
   std::vector<GameObject*> getCollidingObjects(BoundingRectangle bRect);
   void printTree();

   private:
      BVHNode *head;
      std::vector<BVHNode> allNodes;
};

#endif //BVHTREE_H_
