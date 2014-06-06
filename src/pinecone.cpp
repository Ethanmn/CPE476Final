#include "pinecone.h"

#include "ground_plane.h"

#include <glm/gtc/matrix_transform.hpp>

const auto kAreaOfEffect = 30.f;

Pinecone::Pinecone(const Mesh& mesh, const GroundPlane& ground, const glm::vec2& position) :
   draw_template_({
         ShaderType::TEXTURE,
         mesh,
         Texture(TextureType::PINECONE, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({
            EffectType::CASTS_SHADOW,
            EffectType::CASTS_REFLECTION
            })
         }),
   bounding_rectangle_(
         position,
         glm::vec2(mesh.max.x - mesh.min.x, mesh.max.z - mesh.min.z),
         0.0f),
   aoe_bounding_rectangle_(
         position,
         glm::vec2(kAreaOfEffect, kAreaOfEffect),
         0.0f),
   position_(position.x, ground.heightAt(glm::vec3(position.x, 0, position.y)), position.y),
   been_pounced_(false)
{
}

Drawable Pinecone::drawable() const {
   const auto mt = glm::translate(glm::mat4(), position_);
   return Drawable({
         draw_template_,
         std::vector<glm::mat4>({mt})
         });
}
