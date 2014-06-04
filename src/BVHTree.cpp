/* 
   BVHTree.cpp (Bounding Volume Hierarchy Tree)
   Katie Keim
   Deer - CPE 476
*/

#include "BVHTree.h"

void BVHTree::calculateTree(const std::vector<GameObject*>& objects) {
   allNodes = objects;
}

std::vector<GameObject*> BVHTree::getCollidingObjects(const BoundingRectangle& bRect) {
   std::vector<GameObject*> gos;
   for (auto& o : allNodes) {
      if (o->getBoundingRectangle().collidesWith(bRect)) {
         gos.push_back(o);
      }
   }
   return gos;
}
