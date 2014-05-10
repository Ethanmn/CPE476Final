/* 
   TreeGenerator.h
   Katie Keim
   Deer - CPE 476
*/
#ifndef TREE_GEN_H_
#define TREE_GEN_H_

#include "ground_plane.h"
#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "bounding_rectangle.h"
#include "Tree.h"

struct TreeGenerator {
   TreeGenerator(const Mesh& mesh);

   void generate();
   std::vector<Tree> getTrees();
   Tree getCenterTree();

   private:
      std::vector<Tree> trees;
      Mesh treeMesh1;
};

#endif //TREE_GEN_H_
