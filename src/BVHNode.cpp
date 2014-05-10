/* 
   BVHNode.h (Binary Spatial Partitioning Node)
   Katie Keim
   Deer - CPE 476
*/
#include "BVHNode.h"

BVHNode::BVHNode(BoundingRectangle bRect, GameObject *object) :
   object(object),
   bRect(bRect),
   leftNode(NULL),
   rightNode(NULL)
{}

void BVHNode::setLeftNode(BVHNode *left) {
   leftNode = left;
}

void BVHNode::setRightNode(BVHNode *right) {
   rightNode = right;
}

GameObject *BVHNode::getGameObject() {
   return object;
}

BoundingRectangle BVHNode::getRect() {
   return bRect;
}

/* Do not call this if hasLeft() returns false */
BVHNode *BVHNode::getLeftNode() {
   return leftNode;
}

/* Do not call this if hasRight() returns false */
BVHNode *BVHNode::getRightNode() {
   return rightNode;
}

bool BVHNode::hasLeftNode() {
   return leftNode == NULL;
}

bool BVHNode::hasRightNode() {
   return rightNode == NULL;
}
