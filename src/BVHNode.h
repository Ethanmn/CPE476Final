/* 
   BVHNode.h (Bounding Volume Hierarchy Node)
   Katie Keim
   Deer - CPE 476
*/

#ifndef BVHNODE_H_
#define BVHNODE_H_

#include "bounding_rectangle.h"
#include "GameObject.h"

struct BVHNode {
   BVHNode(BoundingRectangle bRect, GameObject *object, int num);

   GameObject *getGameObject();
   BoundingRectangle getRect();

   bool hasLeftNode();
   bool hasRightNode();

   void printNode();

   void setLeftIndex(int left);
   void setRightIndex(int right);

   int getLeftIndex();
   int getRightIndex();

   int num;

   private:
      GameObject *object;
      BoundingRectangle bRect;

      int leftIndex;
      int rightIndex;
};
#endif //BVHNODE_H_
