#include "FlowerGenerator.h"

const int FLOWER_DENSITY = 10; //Larger value means LESS flowers
const int FLOWER_SIZE = 10;

//Need the * 100 to translate between modulo and floats
const float FLOWER_SCALE_MIN = 0.4 * 100;
const float FLOWER_SCALE_MAX = 1.6 * 100;

FlowerGenerator::FlowerGenerator(const Mesh& mesh, const Mesh& mesh_eaten,
      TextureType texture_type, const GroundPlane& ground, float errX, float errY) : 
   draw_template_({
         ShaderType::DEFERRED,
         mesh,
         Material(),
         Texture(texture_type, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({EffectType::CASTS_SHADOW}) }),
   draw_template_eaten_({
         ShaderType::DEFERRED,
         mesh_eaten, 
         Material(),
         Texture(texture_type, DIFFUSE_TEXTURE), 
         boost::none,
         EffectSet({EffectType::CASTS_SHADOW}) })    
{
   draw_template_.material = Material(glm::vec3(0.45, 0.0, 0.45));
   generate(ground, errX, errY);
}

//Generate the flowers
void FlowerGenerator::generate(const GroundPlane& ground,  float errX, float errY) {
   int size = GroundPlane::GROUND_SCALE / FLOWER_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % FLOWER_DENSITY == 0) {
            float scale = (rand() % (int)(FLOWER_SCALE_MAX - FLOWER_SCALE_MIN) + FLOWER_SCALE_MIN)  / 50.0f;
            float x = row * FLOWER_SIZE - groundSize + rand() % FLOWER_SIZE;
            float y = col * FLOWER_SIZE - groundSize + rand() % FLOWER_SIZE;

            flowers.push_back(Flower(draw_template_.mesh, glm::vec3(x, 0.0f, y), ground, scale, errX, errY));
         }
      }
   }
}

std::vector<Flower>& FlowerGenerator::getFlowers() {
   return flowers;
}

Drawable FlowerGenerator::drawable() const {
   std::vector<DrawInstance> model_matrices;
   for(auto& flower : flowers) {
      if(!flower.isEaten())
         model_matrices.push_back(flower.calculateModel());
   }
   return Drawable({draw_template_, model_matrices});
} 

Drawable FlowerGenerator::drawableEaten() const {
   std::vector<DrawInstance> model_matrices;
   for(auto& flower : flowers) {
      if(flower.isEaten())
         model_matrices.push_back(flower.calculateModel());
   }
   return Drawable({draw_template_eaten_, model_matrices});
} 
