#include "pinecone.h"

#include "ground_plane.h"

#include <glm/gtc/matrix_transform.hpp>

Pinecone::Pinecone(const Mesh& mesh, const GroundPlane& ground, const glm::vec2& position) :
   draw_template_({
         ShaderType::TEXTURE,
         mesh,
         Texture(TextureType::DEER, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({
            EffectType::CASTS_SHADOW,
            EffectType::CASTS_REFLECTION
            })
         }),
   position_(position.x, ground.heightAt(glm::vec3(position.x, 0, position.y)), position.y)
{
}

Drawable Pinecone::drawable() const {
   const auto mt = glm::translate(glm::mat4(), position_);
   return Drawable({
         draw_template_,
         std::vector<glm::mat4>({mt})
         });
}
