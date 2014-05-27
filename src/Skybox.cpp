/* 
   Skybox.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Skybox.h"

Skybox::Skybox(Mesh mesh) :
   draw_template_night({
      ShaderType::TEXTURE,
      mesh,
      Texture(TextureType::SKYBOX_DAY, DIFFUSE_TEXTURE),
      boost::none,
      EffectSet({})}),
   draw_template_day({
      ShaderType::TEXTURE,
      mesh,
      Texture(TextureType::SKYBOX_NIGHT, DIFFUSE_TEXTURE),
      boost::none,
      EffectSet({})})
{}

glm::mat4 Skybox::calculateModel() const {
   return glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 40.0f, 40.0f));//glm::vec3(GroundPlane::GROUND_SCALE, GroundPlane::GROUND_SCALE, GroundPlane::GROUND_SCALE));
}

Drawable Skybox::drawableDay() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(calculateModel());
   return Drawable({draw_template_day, model_matrices});
} 

Drawable Skybox::drawableNight() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(calculateModel());
   return Drawable({draw_template_night, model_matrices});
}
