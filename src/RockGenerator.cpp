/* 
   RockGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "RockGenerator.h"

const int ROCK_INVERSE_DENSITY = 75; //Larger value means LESS rcoks

const int ROCK_SIZE = 8;

//Need the * 100 to translate between modulo and floats
const float ROCK_SCALE_MIN = 0.1 * 100;
const float ROCK_SCALE_MAX = 2.5 * 100;

RockGenerator::RockGenerator(const Mesh& mesh, const GroundPlane& ground) : 
   draw_template_({
         ShaderType::DEFERRED,
         mesh,
         Material(),
         Texture(TextureType::ROCK, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION}) }) 
{
   draw_template_.material = Material(glm::vec3(0.45, 0.24, 0.15));
   generate(ground);
}

//Generate the rocks
void RockGenerator::generate(const GroundPlane& ground) {
   int size = GroundPlane::GROUND_SCALE / ROCK_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % ROCK_INVERSE_DENSITY == 0) {
            float scale = (rand() % (int)(ROCK_SCALE_MAX - ROCK_SCALE_MIN) + ROCK_SCALE_MIN)  / 100.0f;
            float x = row * ROCK_SIZE - groundSize + rand() % ROCK_SIZE;
            float y = col * ROCK_SIZE - groundSize + rand() % ROCK_SIZE;
            float angle = rand() % 360;

            rocks.push_back(Rock(draw_template_.mesh, glm::vec3(x, 0.0f, y), angle, ground, scale));
         }
      }
   }
}

std::vector<Rock>& RockGenerator::getRocks() {
   return rocks;
}

Drawable RockGenerator::drawable() const {
   std::vector<DrawInstance> model_matrices;
   for(auto& rock : rocks)
      model_matrices.push_back(rock.calculateModel());
   return Drawable({draw_template_, model_matrices});
} 

