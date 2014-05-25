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
   draw_template_({ShaderType::TEXTURE, mesh, 
         Texture(texture_path(Textures::TREE), DIFFUSE_TEXTURE), boost::none, EffectSet({EffectType::CASTS_REFLECTION}) })
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
            trees.push_back(Tree(glm::vec3(row * TREE_SIZE - groundSize, TREE_SCALE * TREE_SIZE / 2, col * TREE_SIZE - groundSize)));
         }
      }
   }
}

void TreeGenerator::includeInShadows(bool value) {
   if (value)
      draw_template_.effects.insert(EffectType::CASTS_SHADOW);
   else
      draw_template_.effects.erase(EffectType::CASTS_SHADOW);
}

std::vector<Tree>& TreeGenerator::getTrees() {
   return trees;
}

Drawable TreeGenerator::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for (auto& tree : trees) 
      model_matrices.push_back(tree.calculateModel());
   return Drawable({draw_template_, model_matrices});
}
