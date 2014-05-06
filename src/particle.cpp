#include "particle.h"
#include "graphics/shader_setup.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"

void Particle::step(units::MS dt) {
   life_time_ += dt;
   velocity_ += acceleration_ * dt;
   position_ += velocity_ * dt;
}

void Particle::draw(
      Shader& shader,
      const UniformLocationMap& uniform_location_map,
      const glm::mat4& view_matrix) const {
         const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position_));
         const glm::mat4 scale(glm::scale(
            glm::mat4(),
            glm::vec3(scale_)));
         const glm::mat4 rotate(glm::rotate(
            glm::mat4(),
            rotate_,
            glm::vec3(0, 1, 0));
         const glm::mat4 model_matrix(translate * scale * rotate);
   
         setupModelView(shader, uniform_location_map, model_matrix, view_matrix, true);
         sendMaterial(shader, uniform_location_map, glm::vec3(0.45, 0.24, 0.15));
   
         shader.drawMesh(mesh_);
         bounding_rectangle_.draw(uniform_location_map, shader, 0, view_matrix);
      }
)

glm::vec3 Particle::getPos() {
   return posistion_;
}

glm::vec3 Particle::getVel() {
   return velocity_;
}

glm::vec3 Particle::getAccel() {
   return acceleration_;
}

units::MS Particle::getLife() {
   return life_time_;
}
