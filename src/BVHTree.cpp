/* 
   BVHTree.cpp (Bounding Volume Hierarchy Tree)
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
   BVHNode *closestNode;

   glm::vec2 curCenter;
   glm::vec2 otherCenter;

   glm::vec2 closestCenter;
   float closestLength;
   int closestIndex;

   int nodeQSize;
   int numNodes = 0;
   int numLoops = 0;
   int numObjNodes = 0;
   int numObjects = (int)(objects.size());

   float newCenterX;
   float newCenterY;

   GameObject *object;

   //Create a node for every object (bottom of tree).
   for (int index = 0; index < numObjects; index++) {
      object = objects.at(index);
      allNodes.push_back(BVHNode(object->getBoundingRectangle(), object, numNodes++));
      numObjNodes++;
   }

   numObjects = (int)(allNodes.size());

   //Push all the new nodes onto the queue.
   for (int index = 0; index < numObjects; index++) {
      nodeQ.push_back(&(allNodes.at(index)));
   }

   //Loop through all nodes, combining those that are closest, until only 1 is left.
   while (nodeQ.size() > 1) {
      numLoops++;
      curNode = nodeQ.front();
      nodeQ.erase(nodeQ.begin());

      curCenter = curNode->getRect().getCenter();

      closestCenter = nodeQ.front()->getRect().getCenter();
      closestLength = glm::length(curCenter - closestCenter);
      closestIndex = 0;

      nodeQSize = (int)(nodeQ.size());

      //Find the closest node.
      for (int index = 0; index < nodeQSize; index++) {
         otherCenter = nodeQ.at(index)->getRect().getCenter();
         
         if (glm::length(curCenter - otherCenter) < closestLength) {
            closestLength = glm::length(curCenter - otherCenter);
            closestCenter = otherCenter;
            closestIndex = index;
         }
      }

      closestNode = nodeQ.at(closestIndex);

      BoundingRectangle curRect = curNode->getRect();
      BoundingRectangle closestRect = closestNode->getRect();

      //Take the found node out of the queue.
      nodeQ.erase(nodeQ.begin() + closestIndex);
      
      //Combine the two nodes. What if one is inside the other?
      float newDimX = std::abs(curCenter.x - closestCenter.x) + curRect.getDimensions().x / 2.0f + closestRect.getDimensions().x / 2.0f;
      float newDimY = std::abs(curCenter.y - closestCenter.y) + curRect.getDimensions().y / 2.0f + closestRect.getDimensions().y / 2.0f;


      if (curCenter.x > closestCenter.x) {
         newCenterX = curCenter.x - (newDimX / 2.0f - curRect.getDimensions().x / 2.0f);         
      }
      else {
         newCenterX = closestCenter.x - (newDimX / 2.0f - closestRect.getDimensions().x / 2.0f);
      }

      if (curCenter.y > closestCenter.y) {
         newCenterY = curCenter.y - (newDimY / 2.0f - curRect.getDimensions().y / 2.0f);         
      }
      else {
         newCenterY = closestCenter.y - (newDimY / 2.0f - closestRect.getDimensions().y / 2.0f); 
      }

      
      

      if (newDimX < curRect.getDimensions().x) {
         newDimX = curRect.getDimensions().x;
         newCenterX = curCenter.x;
      }

      if (newDimX < closestRect.getDimensions().x) {
         newDimX = closestRect.getDimensions().x;
         newCenterX = closestCenter.x;
      }

      if (newDimY < curRect.getDimensions().y) {
         newDimY = curRect.getDimensions().y;
         newCenterY = curCenter.y;
      }

      if (newDimY < closestRect.getDimensions().y) {
         newDimY = closestRect.getDimensions().y;
         newCenterY = closestCenter.y;
      }

       glm::vec2 newCenter = glm::vec2(newCenterX, newCenterY);

      int curIndex = curNode->num;
      int otherIndex = closestNode->num;

      //Set the left and right nodes of this new combined node.
      allNodes.push_back(BVHNode(BoundingRectangle(newCenter, glm::vec2(newDimX, newDimY), 0.0f), NULL, numNodes++));
      allNodes.back().setLeftIndex(curIndex);
      allNodes.back().setRightIndex(otherIndex);

      //Push the new combined node back onto the queue.
      nodeQ.push_back(&(allNodes.back()));
   }

   //One node should be left. This is the head.
   if (!nodeQ.empty()) {
      head = nodeQ.front();
   }
}

std::vector<GameObject*> BVHTree::getCollidingObjects(BoundingRectangle bRect) {
   std::vector<GameObject*> collObjs;
   std::list<BVHNode*> nodeQ;
   BVHNode *curNode;
   int numChecks = 0;

   nodeQ.push_back(head);  

   while (!nodeQ.empty()) {
      numChecks++;
      curNode = nodeQ.front();
      nodeQ.pop_front();
      
      if (curNode->getRect().collidesWith(bRect)) {
         
         if (curNode->getGameObject() != NULL) {
            collObjs.push_back(curNode->getGameObject());
         }

         if (curNode->hasLeftNode()) {
            nodeQ.push_back(&(allNodes.at(curNode->getLeftIndex())));
         }
         
         if (curNode->hasRightNode()) {
            nodeQ.push_back(&(allNodes.at(curNode->getRightIndex())));
         }
      }
   }

   return collObjs;
}

void BVHTree::printTree() {
   std::vector<BVHNode*> nodeQ;
   BVHNode *curNode;
   nodeQ.push_back(head);

   while (nodeQ.size() > 0) {
      curNode = nodeQ.at(0);
      nodeQ.erase(nodeQ.begin());
      curNode->printNode();

      if (curNode->hasLeftNode()) {
            //printf("Adding left node. ");
            nodeQ.push_back(&(allNodes.at(curNode->getLeftIndex())));
            //curNode->getLeftNode()->printNode();
         }
         
         if (curNode->hasRightNode()) {
            //printf("Adding right node. ");
            nodeQ.push_back(&(allNodes.at(curNode->getRightIndex())));
            //curNode->getRightNode()->printNode();
         }
   }
}
