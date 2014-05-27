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
   glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(GroundPlane::GROUND_SCALE + 2.0f, 2.0f, GroundPlane::GROUND_SCALE + 2.0f));

   //glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));

   return scale;// * translate;
}

std::vector<Drawable> Skybox::drawables(bool isDay) const {
   std::vector<glm::mat4> model_matrices;
   std::vector<Drawable> drawables;
   glm::mat4 planeMat = calculateModel();

   for (int side = 0; side < 4; side++) {
      glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
      rotation = glm::rotate(glm::mat4(1.0f), 90.0f * side, glm::vec3(0.0f, 1.0f, 0.0f)) * rotation;
      glm::mat4 translate;

      if (side % 2) {
         int multiply = side <= 1 ? -1 : 1;
         translate = glm::translate(glm::mat4(1.0f), glm::vec3(multiply * 20.0f, 0.0f, 0.0f));
      }
      else {
         int multiply = side <= 1 ? -1 : 1;
         translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, multiply * 20.0f));
      }

      model_matrices.clear();
      model_matrices.push_back(translate * rotation * planeMat);

      if (isDay) {
         drawables.push_back(Drawable({draw_template_day, model_matrices}));
      }
      else {
         drawables.push_back(Drawable({draw_template_night, model_matrices}));
      }
   }

   return drawables;
} 
