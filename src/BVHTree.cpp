/* 
   BVHTree.cpp (Binary Spatial Partitioning Tree)
   Katie Keim
   Deer - CPE 476
*/

#include "BVHTree.h"

BVHTree::BVHTree(std::vector<GameObject*> objects) {
   std::vector<BVHNode*> nodeQ;
   BVHNode newNode = BVHNode(BoundingRectangle(glm::vec2(0, 0), glm::vec2(0, 0), 0), NULL);
   BVHNode *curNode;
   BVHNode *otherNode;
   glm::vec2 curCenter;
   glm::vec2 otherCenter;
   glm::vec2 closestCenter;
   float closestLength;
   int closestIndex;

   for (int index = 0; index < objects.size(); index++) {
      BVHNode node = BVHNode(objects.at(index)->getBoundingRectangle(), objects.at(index));
      nodeQ.push_back(&node);
   }

   while (nodeQ.size() > 1) {
      curNode = nodeQ.at(0);
      curCenter = curNode->getRect().getCenter();
      nodeQ.erase(nodeQ.begin());

      closestCenter = nodeQ.at(0)->getRect().getCenter();
      closestLength = glm::length(curCenter - closestCenter);
      closestIndex = 0;

      for (int index = 0; index < nodeQ.size(); index++) {
         otherCenter = nodeQ.at(index)->getRect().getCenter();
         
         if (glm::length(curCenter - otherCenter) < closestLength) {
            closestLength = glm::length(curCenter - otherCenter);
            closestCenter = otherCenter;
            closestIndex = index;
         }
      }

      otherNode = nodeQ.at(closestIndex);

      BoundingRectangle curRect = curNode->getRect();
      BoundingRectangle closestRect = otherNode->getRect();

      nodeQ.erase(nodeQ.begin() + closestIndex);
      glm::vec2 newCenter = glm::vec2((curCenter.x + closestCenter.x) / 2, (curCenter.y + closestCenter.y) / 2);

      float newDimX = glm::dot(curCenter - otherCenter, glm::vec2(1, 0)) + curRect.getDimensions().x / 2 + closestRect.getDimensions().x / 2;
      float newDimY = glm::dot(curCenter - otherCenter, glm::vec2(0, 1)) + curRect.getDimensions().y / 2 + closestRect.getDimensions().y / 2;

      newNode = BVHNode(BoundingRectangle(newCenter, glm::vec2(newDimX, newDimY), 0), NULL);
      newNode.setLeftNode(curNode);
      newNode.setRightNode(otherNode);
      nodeQ.push_back(&newNode);
   }

   if (nodeQ.size() > 0) {
      head = nodeQ.at(0);
   }
}

std::vector<GameObject*> BVHTree::getCollidingObjects(BoundingRectangle bRect) {
   std::vector<GameObject*> collObjs;
   std::vector<BVHNode*> nodeQ;
   BVHNode *curNode;
   nodeQ.push_back(head);

   while (nodeQ.size() > 0) {
      curNode = nodeQ.at(0);
       nodeQ.erase(nodeQ.begin());
      if (curNode->getGameObject() != NULL) {
         collObjs.push_back(curNode->getGameObject());
      }

      if (curNode->hasLeftNode() && curNode->getLeftNode()->getRect().collidesWith(bRect)) {
         nodeQ.push_back(curNode->getLeftNode());
      }
         
      if (curNode->hasRightNode() && curNode->getRightNode()->getRect().collidesWith(bRect)) {
         nodeQ.push_back(curNode->getRightNode());
      }
   }

   return collObjs;
}

void BVHTree::drawAll(Shader& shader, const UniformLocationMap& uniform_location_map, const glm::mat4& view_matrix) const {
   std::vector<BVHNode*> nodeQ;
   BVHNode *curNode;
   nodeQ.push_back(head);

   while (nodeQ.size() > 0) {
     curNode = nodeQ.at(0);
       nodeQ.erase(nodeQ.begin());
      if (curNode->getGameObject() != NULL) {
         curNode->getGameObject()->draw(shader, uniform_location_map, view_matrix);
      }
      if (curNode->hasLeftNode()) {
         nodeQ.push_back(curNode->getLeftNode());
      }
      if (curNode->hasRightNode()) {
         nodeQ.push_back(curNode->getRightNode());
      }
   }
}
