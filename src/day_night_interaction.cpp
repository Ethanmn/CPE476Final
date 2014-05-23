#include "day_night_interaction.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"
#include "graphics/material.h"

Drawable DayNightInteraction::drawableSun() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(sun_transform_);
   return Drawable({draw_template_sun_, model_matrices});
}

Drawable DayNightInteraction::drawableMoon() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(moon_transform_);
   return Drawable({draw_template_moon_, model_matrices});
} 
