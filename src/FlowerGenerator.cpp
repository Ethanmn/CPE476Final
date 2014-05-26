#include "FlowerGenerator.h"

const int FLOWER_DENSITY = 10; //Larger value means LESS bushes
const int FLOWER_SIZE = 10;

//Need the * 100 to translate between modulo and floats
const float FLOWER_SCALE_MIN = 0.8 * 100;
const float FLOWER_SCALE_MAX = 1.3 * 100;

FlowerGenerator::FlowerGenerator(const Mesh& mesh, TextureType texture_type) : 
   draw_template_({ShaderType::TEXTURE, mesh, 
         Texture(texture_type, DIFFUSE_TEXTURE), boost::none, EffectSet() }) 
{
   draw_template_.mesh.material = Material(glm::vec3(0.45, 0.0, 0.45));
}

//Generate the trees
void FlowerGenerator::generate(const GroundPlane& ground) {
   int size = GroundPlane::GROUND_SCALE / FLOWER_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % FLOWER_DENSITY == 0) {
            float scale = (rand() % (int)(FLOWER_SCALE_MAX - FLOWER_SCALE_MIN) + FLOWER_SCALE_MIN)  / 50.0f;
            glm::vec3 pos = glm::vec3(row * FLOWER_SIZE - groundSize,  0, col * FLOWER_SIZE - groundSize);
            flowers.push_back(Flower(draw_template_.mesh, pos, ground, scale));
         }
      }
   }
}

std::vector<Flower>& FlowerGenerator::getFlowers() {
   return flowers;
}

Drawable FlowerGenerator::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for(auto& flower : flowers)
      model_matrices.push_back(flower.calculateModel());
   return Drawable({draw_template_, model_matrices});
} 


