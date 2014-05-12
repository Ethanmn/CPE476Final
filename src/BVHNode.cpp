/* 
   BVHNode.h (Binary Spatial Partitioning Node)
   Katie Keim
   Deer - CPE 476
*/
#include "BVHNode.h"

BVHNode::BVHNode(BoundingRectangle bRect, GameObject *object, int num) :
   object(object),
   bRect(bRect),
   num(num),
   leftIndex(-1),
   rightIndex(-1)
{ 
//printNode(); 
}

GameObject *BVHNode::getGameObject() {
   return object;
}

BoundingRectangle BVHNode::getRect() {
   return bRect;
}

bool BVHNode::hasLeftNode() {
   return leftIndex >= 0;//leftNode != NULL;
}

bool BVHNode::hasRightNode() {
   return rightIndex >= 0;//rightNode != NULL;
}

void BVHNode::printNode() {
   printf("#%d node with center at (%f, %f) and dimensions (%f, %f)", num, bRect.getCenter().x, bRect.getCenter().y, bRect.getDimensions().x, bRect.getDimensions().y);

   if (object == NULL) {
      printf(" NO GO.\n");
   }
   else {
      printf(" YES GO.\n");
   }

   if (hasLeftNode()) {
      printf("LEFT NODE %d ", leftIndex);
   }

   if (hasRightNode()) {
      printf("RIGHT NODE %d", rightIndex);
   }

   printf("\n");
}

void BVHNode::setRightIndex(int right) {
   rightIndex = right;
}

void BVHNode::setLeftIndex(int left) {
   leftIndex = left;
}

int BVHNode::getLeftIndex() {
   return leftIndex;
}

int BVHNode::getRightIndex() {
   return rightIndex;
}
