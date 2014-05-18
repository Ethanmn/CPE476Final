/* 
   BushGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "BushGenerator.h"

const int BUSH_DENSITY = 6; //Larger value means LESS bushes

const int BUSH_SIZE = 10;

//Need the * 100 to translate between modulo and floats
const float BUSH_SCALE_MIN = 0.8 * 100;
const float BUSH_SCALE_MAX = 1.3 * 100;

const int BUSH_RUSTLE_MIN = 150;
const int BUSH_RUSTLE_MAX = 450;

BushGenerator::BushGenerator(const Mesh& mesh) : 
   draw_template_({ShaderType::SUN, mesh, boost::none, boost::none, true }) 
{
   draw_template_.mesh.material = Material(glm::vec3(0.45, 0.24, 0.15));
}

//Generate the trees
void BushGenerator::generate(const GroundPlane& ground) {
   int size = GroundPlane::GROUND_SCALE / BUSH_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % BUSH_DENSITY == 0) {
            float scale = (rand() % (int)(BUSH_SCALE_MAX - BUSH_SCALE_MIN) + BUSH_SCALE_MIN)  / 100.0f;
            glm::vec3 pos = glm::vec3(row * BUSH_SIZE - groundSize,  0, col * BUSH_SIZE - groundSize);
            bushes.push_back(Bush(draw_template_.mesh, 
               pos, ground, scale, rand() % (int)(BUSH_RUSTLE_MAX - BUSH_RUSTLE_MIN) + BUSH_RUSTLE_MIN));
         }
      }
   }
}

std::vector<Bush>& BushGenerator::getBushes() {
   return bushes;
}

Drawable BushGenerator::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for(auto& bush : bushes)
      model_matrices.push_back(bush.calculateModel());
   return Drawable({draw_template_, model_matrices});
} 

