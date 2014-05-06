/* 
   BVHNode.h (Bounding Volume Hierarchy Node)
   Katie Keim
   Deer - CPE 476
*/
#ifndef BVHNODE_H_
#define BVHNODE_H_

#include "bounding_rectangle.h"

struct BVHNode {
   BVHNode(BoundingRectangle bRect);

   BoundingRectangle getRect();

   void setLeftNode(BVHNode *left);
   void setRightNode(BVHNode *right);
   BVHNode getLeftNode();
   BVHNode getRightNode();
   bool hasLeft();
   bool hasRight();

   private:
      BoundingRectangle bRect;

      BVHNode *leftNode;
      BVHNode *rightNode;

      bool leftSet;
      bool rightSet;
};
#endif //BVHNODE_H_
