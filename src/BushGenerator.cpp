/* 
   BushGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "BushGenerator.h"

const int BUSH_INVERSE_DENSITY = 10; //Larger value means LESS bushes

const int BUSH_SIZE = 10;

//Need the * 100 to translate between modulo and floats
const float BUSH_SCALE_MIN = 0.4 * 100;
const float BUSH_SCALE_MAX = 1.8 * 100;

const int BUSH_RUSTLE_MIN = 250;
const int BUSH_RUSTLE_MAX = 450;

BushGenerator::BushGenerator(const Mesh& mesh, const GroundPlane& ground) : 
   draw_template_({
         ShaderType::DEFERRED,
         mesh,
         Material(),
         Texture(TextureType::TREE, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION, EffectType::VARY_MATERIAL}) }) 
{
   draw_template_.material = Material(glm::vec3(0.45, 0.24, 0.15));
   generate(ground);
}

//Generate the bushes
void BushGenerator::generate(const GroundPlane& ground) {
   int size = GroundPlane::GROUND_SCALE / BUSH_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % BUSH_INVERSE_DENSITY == 0) {
            float scale = (rand() % (int)(BUSH_SCALE_MAX - BUSH_SCALE_MIN) + BUSH_SCALE_MIN)  / 100.0f;
            float x = row * BUSH_SIZE - groundSize + rand() % BUSH_SIZE;
            float y = col * BUSH_SIZE - groundSize + rand() % BUSH_SIZE;
            float angle = rand() % 360;
            float rustleTime = (rand() % (int)(BUSH_RUSTLE_MAX - BUSH_RUSTLE_MIN)) + BUSH_RUSTLE_MIN;

            if (ground.heightAt(glm::vec3(x, 0, y)) > 0.f) {
               bushes.push_back(Bush(draw_template_.mesh, glm::vec3(x, 0.0f, y), angle, ground, scale, rustleTime));
            }
         }
      }
   }
}

std::vector<Bush>& BushGenerator::getBushes() {
   return bushes;
}

Drawable BushGenerator::drawable() const {
   std::vector<DrawInstance> model_matrices;
   for(auto& bush : bushes)
      model_matrices.push_back(bush.calculateModel());
   return Drawable({draw_template_, model_matrices});
} 

