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
   TreeGenerator(const Mesh& mesh, MeshLoader mesh_loader, AttributeLocationMap attribute_location_map);

   void generate();
   std::vector<Tree>& getTrees();
   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;
   void includeInShadows(bool value); 

   private:
      std::vector<Tree> trees;
      DrawTemplate draw_template_;
      MeshLoader mesh_loader_;
      AttributeLocationMap attribute_location_map_;
};

#endif //TREE_GEN_H_
