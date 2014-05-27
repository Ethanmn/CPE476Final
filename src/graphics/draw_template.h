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
};

typedef std::set<EffectType> EffectSet;

struct DrawTemplate {
   ShaderType shader_type;
   Mesh mesh;
   boost::optional<Texture> texture;
   boost::optional<Texture> height_map;
   bool has_bones() const { return !mesh.bone_array.empty(); }
   EffectSet effects;
   bool is_god_ray;
};

struct Drawable {
   DrawTemplate draw_template;
   std::vector<glm::mat4> model_transforms;
};
void switchTextureAndBlinn(Drawable *drawObj);

#endif
