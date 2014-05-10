/* 
   BVHTree.h (Binary Spatial Partitioning Tree)
   Katie Keim
   Deer - CPE 476
*/

#ifndef BVHTREE_H_
#define BVHTREE_H_

#include "bounding_rectangle.h"
#include "GameObject.h"
#include "BVHNode.h"

struct BVHTree {
   BVHTree(std::vector<GameObject*> objects);

   std::vector<GameObject*> getCollidingObjects(BoundingRectangle bRect);
   void drawAll(Shader& shader, const UniformLocationMap& uniform_location_map, const glm::mat4& view_matrix) const;

   private:
      BVHNode *head;
};

#endif //BVHTREE_H_
