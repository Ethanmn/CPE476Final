/* 
   TreeGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "TreeGenerator.h"

const int TREE_SIZE = 20;
const int TREE_DENSITY = 4; //Higher numbers here will mean less trees.
const int TREE_SCALE = 5;

TreeGenerator::TreeGenerator(const Mesh& mesh) :
   draw_template_({ShaderType::SUN, mesh, boost::none})
{
   draw_template_.mesh.material = Material(glm::vec3(1.2) * glm::vec3(0.45, 0.24, 0.15));
   
}

//Generate the trees
void TreeGenerator::generate() {
   int size = GroundPlane::GROUND_SCALE / TREE_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % TREE_DENSITY == 0) {
            trees.push_back(Tree(glm::vec3(row * TREE_SIZE - groundSize, TREE_SCALE * TREE_SIZE / 2, col * TREE_SIZE - groundSize), draw_template_.mesh));
         }
      }
   }
}

std::vector<Tree>& TreeGenerator::getTrees() {
   return trees;
}

void TreeGenerator::shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, bool betterShadow) {
   for (auto& tree : trees) {
      tree.shadowDraw(shader, uniform_locations, sunDir, betterShadow);
   }
}

void TreeGenerator::drawTrees(Shader& shader, const UniformLocationMap& uniform_location_map, const glm::mat4& view_matrix) {
   for (auto& tree : trees) {
      tree.draw(shader, uniform_location_map, view_matrix);
   }
}

Drawable TreeGenerator::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for (auto& tree : trees) 
      model_matrices.push_back(tree.calculateModel());
   return Drawable({draw_template_, model_matrices});
} 
