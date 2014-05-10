/* 
   BVHNode.h (Binary Spatial Partitioning Node)
   Katie Keim
   Deer - CPE 476
*/

#ifndef BVHNODE_H_
#define BVHNODE_H_

#include "bounding_rectangle.h"
#include "GameObject.h"

struct BVHNode {
   BVHNode(BoundingRectangle bRect, GameObject *object);

   GameObject *getGameObject();
   BoundingRectangle getRect();

   void setLeftNode(BVHNode *left);
   void setRightNode(BVHNode *right);
   BVHNode *getLeftNode();
   BVHNode *getRightNode();
   bool hasLeftNode();
   bool hasRightNode();

   private:
      GameObject *object;
      BoundingRectangle bRect;

      BVHNode *leftNode;
      BVHNode *rightNode;
};
#endif //BVHNODE_H_
