/* 
   Skybox.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Skybox.h"
#include "globals.h"

Skybox::Skybox(Mesh mesh) :
   draw_template_day({
      ShaderType::SKYBOX,
      mesh,
      Material(),
      Texture(TextureType::SKYBOX_DAY, DIFFUSE_TEXTURE),
      boost::none,
      EffectSet({})}),
   draw_template_night({
      ShaderType::SKYBOX,
      mesh,
      Material(),
      Texture(TextureType::SKYBOX_NIGHT, DIFFUSE_TEXTURE),
      boost::none,
      EffectSet({})})
{}

glm::mat4 Skybox::calculateModel(const glm::vec3& cam_pos) const {
   const auto translate = glm::translate(glm::mat4(), cam_pos);
   const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(kFar));
   return translate * scale;
}

Drawable Skybox::drawable(bool isDay, const glm::vec3& cam_pos) const {
   std::vector<DrawInstance> model_matrices;
   model_matrices.push_back(calculateModel(cam_pos));
   if (isDay) {
      return Drawable({draw_template_day, model_matrices});
   }
   else {
      return Drawable({draw_template_night, model_matrices});
   }
}
