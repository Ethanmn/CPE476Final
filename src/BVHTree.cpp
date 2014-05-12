/* 
   BVHTree.cpp (Binary Spatial Partitioning Tree)
   Katie Keim
   Deer - CPE 476
*/

#include "BVHTree.h"

BVHTree::BVHTree() :
head(NULL)
{}

void BVHTree::calculateTree(std::vector<GameObject*> objects) {
   std::vector<BVHNode*> nodeQ;
   BVHNode *curNode;
   BVHNode *otherNode;

   glm::vec2 curCenter;
   glm::vec2 otherCenter;

   glm::vec2 closestCenter;
   float closestLength;
   int closestIndex;

   int numNodes = 0;

   int numLoops = 0;

   for (int index = 0; index < (int)(objects.size()); index++) {
      allNodes.push_back(BVHNode((objects.at(index))->getBoundingRectangle(), objects.at(index), numNodes++));
      //printf("Obj center: (%f, %f)\n", objects.at(index)->getBoundingRectangle().getCenter().x, objects.at(index)->getBoundingRectangle().getCenter().y); 
   }

   for (auto& node : allNodes) {
      nodeQ.push_back(&node);
   }

   while (nodeQ.size() > 1) {
      numLoops++;
      curNode = nodeQ.at(0);
      nodeQ.erase(nodeQ.begin());

      curCenter = curNode->getRect().getCenter();

      closestCenter = nodeQ.at(0)->getRect().getCenter();
      closestLength = glm::length(curCenter - closestCenter);
      closestIndex = 0;

      for (int index = 1; index < (int)(nodeQ.size()); index++) {
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

      float newDimX = std::abs((curCenter - otherCenter).x) + curRect.getDimensions().x / 2 + closestRect.getDimensions().x / 2;
      float newDimY = std::abs((curCenter - otherCenter).y) + curRect.getDimensions().y / 2 + closestRect.getDimensions().y / 2;

      allNodes.push_back(BVHNode(BoundingRectangle(newCenter, glm::vec2(newDimX, newDimY), 0), NULL, numNodes++));
      allNodes.at(allNodes.size() - 1).setLeftNode(curNode);
      allNodes.at(allNodes.size() - 1).setRightNode(otherNode);

      nodeQ.push_back(&(allNodes.at(allNodes.size() - 1)));

      //printf("Cur node: %d\n", curNode); curNode->printNode();
      //printf("Other node: %d\n", otherNode); otherNode->printNode();
   }

   if (nodeQ.size() > 0) {
      head = nodeQ.at(0);
      //head->printNode();
   }

   //printf("Ran through loop %d times with %zu objects.\n", numLoops, objects.size());
}

std::vector<GameObject*> BVHTree::getCollidingObjects(BoundingRectangle bRect) {
   std::vector<GameObject*> collObjs;
   std::vector<BVHNode*> nodeQ;
   BVHNode *curNode;
   nodeQ.push_back(head);
   int numChecks = 0;

   printf("Checking for collision.\n");

   while (nodeQ.size() > 0) {
      printf("Check #%d\n", numChecks);
      numChecks++;
      curNode = nodeQ.at(0);
      nodeQ.erase(nodeQ.begin());
      printf("Node %d popped.\n", curNode->num);

      if (curNode->getRect().collidesWith(bRect)) {
         if (curNode->getGameObject() != NULL) {
            printf("Hit an object.\n");
            collObjs.push_back(curNode->getGameObject());
         }

         if (curNode->hasLeftNode()) {
            printf("Adding left node.\n");
            nodeQ.push_back(curNode->getLeftNode());
         }
         
         if (curNode->hasRightNode()) {
            printf("Adding right node.\n");
            nodeQ.push_back(curNode->getRightNode());
         }
      }
   }

   printf("Checked %d nodes. Found %zu collisions.\n", numChecks, collObjs.size());

   return collObjs;
}

/*void BVHTree::printTree() {
   std::vector<BVHNode*> nodeQ;
   BVHNode *curNode;
   nodeQ.push_back(head);

   while (nodeQ.size() > 0) {
      curNode = nodeQ.at(0);
      nodeQ.erase(nodeQ.begin());
      curNode->printNode();
   }
}*/
