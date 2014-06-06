#include "draw_template.h"

//static
Drawable Drawable::fromCulledDrawable(const CulledDrawable& cd, CullType cull_type) {
   Drawable d;
   d.draw_template = cd.draw_template;
   for (auto& mt : cd.draw_instances) {
      if (!mt.cullFlag.count(cull_type)) {
         d.draw_instances.push_back(mt.model);
      }
   }
   return d;
}
