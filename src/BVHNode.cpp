/* 
   BVHNode.h (Bounding Volume Hierarchy Node)
   Katie Keim
   Deer - CPE 476
*/
#include "BVHNode.h"

BVHNode::BVHNode(BoundingRectangle rect) :
   bRect(rect),
   leftNode(NULL),
   rightNode(NULL),
   leftSet(false),
   rightSet(false)
{}

void BVHNode::setLeftNode(BVHNode *left) {
   leftNode = left;
   leftSet = true;
}

void BVHNode::setRightNode(BVHNode *right) {
   rightNode = right;
   rightSet = true;
}

BoundingRectangle BVHNode::getRect() {
   return bRect;
}

/* Do not call this if hasLeft() returns false */
BVHNode BVHNode::getLeftNode() {
   return *leftNode;
}

/* Do not call this if hasRight() returns false */
BVHNode BVHNode::getRightNode() {
   return *rightNode;
}

bool BVHNode::hasLeft() {
   return leftSet;
}

bool BVHNode::hasRight() {
   return rightSet;
}
