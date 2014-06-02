#include "draw_template.h"

//static
Drawable Drawable::fromCulledDrawable(const CulledDrawable& cd, CullType cull_type) {
   Drawable d;
   d.draw_template = cd.draw_template;
   for (auto& mt : cd.model_transforms) {
      if (!mt.cullFlag.count(cull_type)) {
         d.model_transforms.push_back(mt.model);
      }
   }
   return d;
}
