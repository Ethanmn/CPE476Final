#include "lightning_interaction.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/material.h"

glm::mat4 LightningInteraction::calculateModel() const {
   return glm::mat4(1.0);
}
Drawable LightningInteraction::drawable() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(calculateModel());
   return Drawable({draw_template_, model_matrices}); 
}


