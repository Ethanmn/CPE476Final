#include "pinecone.h"

#include "ground_plane.h"

#include <glm/gtc/matrix_transform.hpp>

const auto kAreaOfEffect = 30.f;

Pinecone::Pinecone(const Mesh& mesh, const GroundPlane& ground, const glm::vec2& position) :
   draw_template_({
         ShaderType::DEFERRED,
         mesh,
         Material(),
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
   position_(position.x, -mesh.min.y + ground.heightAt(glm::vec3(position.x, 0, position.y)), position.y),
   been_pounced_(false)
{
}

Drawable Pinecone::drawable() const {
   const auto mt = glm::translate(glm::mat4(), position_);
   return Drawable({
         draw_template_,
         std::vector<DrawInstance>({mt})
         });
}

void Pinecone::kick(const glm::vec2& direction) {
   velocity_ = glm::normalize(direction) / 20.f;
}

void Pinecone::step(float dt, const GroundPlane& ground) {
   if (glm::length(velocity_) > 0.001f) {
      const auto deltaV = -glm::normalize(velocity_) * 0.00005f * dt;
      if (glm::length(deltaV) > glm::length(velocity_)) {
         velocity_ = glm::vec2();
      } else {
         velocity_ += deltaV;
      }
   }
   const auto dp = velocity_ * dt;
   position_.x += dp.x;
   position_.z += dp.y;
   position_.y = -draw_template_.mesh.min.y + ground.heightAt(position_);
   bounding_rectangle_.set_position(glm::vec2(position_.x, position_.z));
}
