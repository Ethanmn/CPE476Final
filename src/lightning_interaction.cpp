#include "lightning_interaction.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/material.h"

glm::mat4 Lightning::calculateModel() const {
   const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position_));
   const glm::mat4 scale(glm::scale(
            glm::mat4(),
            glm::vec3(scale_)));
   const glm::mat4 rotate(glm::rotate(
            glm::mat4(),
            rotate_,
            glm::vec3(0, 1, 0)));
   return glm::mat4(translate * scale * rotate);
}

Drawable Lightning::drawable() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(calculateModel());
   return Drawable({draw_template_, model_matrices}); 
}


