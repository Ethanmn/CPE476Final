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
   IS_GOD_RAY
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

struct CulledDrawable;
struct Drawable {
   static Drawable fromCulledDrawable(const CulledDrawable& d, CullType cull_type);
   DrawTemplate draw_template;
   std::vector<glm::mat4> model_transforms;
};
void switchTextureAndBlinn(Drawable *drawObj);

struct CulledTransform {
   glm::mat4 model;
   CullSet cullFlag;
};

struct CulledDrawable {
   static CulledDrawable fromDrawable(const Drawable& d) {
      CulledDrawable cd;
      cd.draw_template = d.draw_template;
      for (const auto& mt : d.model_transforms) {
         cd.model_transforms.push_back(CulledTransform({mt, CullSet()}));
      }
      return cd;
   }
   DrawTemplate draw_template;
   std::vector<CulledTransform> model_transforms;
};

#endif
