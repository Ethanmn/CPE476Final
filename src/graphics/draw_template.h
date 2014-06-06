#ifndef DRAW_TEMPLATE_H_
#define DRAW_TEMPLATE_H_

#include <glm/glm.hpp>
#include <set>
#include <boost/optional.hpp>
#include "texture.h"
#include "shaders.h"
#include "mesh.h"

struct Material;
struct Bone;

enum class EffectType {
   CASTS_SHADOW,
   CASTS_REFLECTION,
   VARY_MATERIAL,
   IS_GOD_RAY,
   IS_FIREFLY
};

typedef std::set<EffectType> EffectSet;

enum class CullType {
   REFLECT_CULLING,
   VIEW_CULLING,
   SHADOW_CULLING,
};

typedef std::set<CullType> CullSet;

struct DrawTemplate {
   ShaderType shader_type;
   Mesh mesh;
   Material material;
   boost::optional<Texture> texture;
   boost::optional<Texture> height_map;
   bool has_bones() const { return !mesh.bone_array.empty(); }
   EffectSet effects;
};

struct DrawInstance {
   DrawInstance(const glm::mat4& model_transform=glm::mat4(), const boost::optional<Material>& material=boost::none) :
      model_transform(model_transform),
      material(material)
      {}
   glm::mat4 model_transform;
   boost::optional<Material> material;
};

struct CulledDrawable;
struct Drawable {
   static Drawable fromCulledDrawable(const CulledDrawable& d, CullType cull_type);
   DrawTemplate draw_template;
   std::vector<DrawInstance> draw_instances;
};
void switchTextureAndBlinn(Drawable *drawObj);

struct CulledDrawInstance {
   DrawInstance instance;
   CullSet cullFlag;
};

struct CulledDrawable {
   static CulledDrawable fromDrawable(const Drawable& d) {
      CulledDrawable cd;
      cd.draw_template = d.draw_template;
      for (const auto& mt : d.draw_instances) {
         cd.draw_instances.push_back(CulledDrawInstance({mt, CullSet()}));
      }
      return cd;
   }
   DrawTemplate draw_template;
   std::vector<CulledDrawInstance> draw_instances;
};

#endif
