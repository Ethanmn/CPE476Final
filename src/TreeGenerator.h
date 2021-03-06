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
#include "graphics/draw_template.h"

struct TreeGenerator {
   TreeGenerator(const Mesh& mesh, const Mesh& leaf, const GroundPlane& ground);

   std::vector<Tree>& getTrees();
   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;
   Drawable leafDrawable() const;
   void includeInShadows(bool value); 

  private:
   void generate();
   std::vector<Tree> trees;
   std::vector<int> density_levels;
   DrawTemplate draw_template_;
   Mesh leaf_;
};

#endif //TREE_GEN_H_
