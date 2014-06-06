#include "god_rays.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/material.h"

glm::mat4 GodRays::calculateModel(glm::vec3 position, glm::vec3 scaleAmount) const {
   const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position));
   const glm::mat4 scale(glm::scale(
            glm::mat4(),
            glm::vec3(scale_, 10.0f, scale_)));
   const glm::mat4 shrinking_scale(glm::scale(
            glm::mat4(),
            scaleAmount));
   const glm::mat4 rotate(glm::rotate(
            glm::mat4(),
            rotate_,
            glm::vec3(0, 1, 0)));
   return glm::mat4(translate * shrinking_scale * scale * rotate);
}

void GodRays::setRayPositions(glm::vec2 current_ray, glm::vec2 next_ray) {
   position_ = glm::vec3(current_ray.x, 0, current_ray.y);
   next_position_ = glm::vec3(next_ray.x, 0, next_ray.y);
}

void GodRays::setCurrentRayScale(float scale_amount) {
   current_scale_ = glm::vec3(scale_amount, 1.0, scale_amount);
}

Drawable GodRays::drawable() const {
   std::vector<DrawInstance> model_matrices;
   model_matrices.push_back(calculateModel(position_, current_scale_));
   model_matrices.push_back(calculateModel(next_position_, glm::vec3(1.0)));
   return Drawable({draw_template_, model_matrices}); 
}


