#ifndef CULLED_DRAWABLE_H_
#define CULLED_DRAWABLE_H_

#include "graphics/draw_template.h"

#include <glm/glm.hpp>

enum class CullType {
   REFLECT_CULLING,
   VIEW_CULLING
};

typedef std::set<CullType> CullSet;

struct CulledTransform {
   glm::mat4 model;
   CullSet cullFlag;
};

struct CulledDrawable {
   DrawTemplate draw_template;
   std::vector<CulledTransform> model_transforms;
};

#endif
