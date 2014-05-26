#include "SongStoneGenerator.h"

const glm::vec3 CenterOfCircle = glm::vec3(-115.0f, 0.0f, -70.0f);
const float CIRCLE_RADIUS = 20.0f;
const int NUM_STONES = 5;
const float PI = 3.1415926;

SongStoneGenerator::SongStoneGenerator(const Mesh& mesh) : 
   draw_template_({ShaderType::TEXTURE, mesh, boost::none, boost::none, EffectSet()}) 
{
   draw_template_.mesh.material = Material(glm::vec3(0.45, 0.0, 0.45));
}

//Generate the trees
void SongStoneGenerator::generate(const GroundPlane& ground) {
   float angle = 2*PI / NUM_STONES;
   for (int i = 0; i < NUM_STONES; i++) {
      float scale = 1.0f;
      glm::vec3 pos = glm::vec3(CenterOfCircle.x + CIRCLE_RADIUS * sin(angle * i),  
                                0, 
                                CenterOfCircle.x + CIRCLE_RADIUS * cos(angle * i));
      stones.push_back(SongStone(draw_template_.mesh, pos, ground, scale));
   }
}

std::vector<SongStone>& SongStoneGenerator::getSongStones() {
   return stones;
}

Drawable SongStoneGenerator::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for(auto& stone : stones)
      model_matrices.push_back(stone.calculateModel());
   return Drawable({draw_template_, model_matrices});
} 


