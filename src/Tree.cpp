/*
   Tree.cpp
   Katie Keim
   CPE 476 - Deer
   */
#include "Tree.h"
#include <iostream>

const int TREE_SCALE = 3;

const int BOUNDING_SIZE = 4;
const float BOUNDING_ERR_X = 0.15f;
const float BOUNDING_ERR_Z = 1.0f;

Tree::Tree(glm::vec3 position, float heightOffset, float widthOffset, float angleRot, const Mesh& leaf) :
   leaf_system_(leaf, TextureType::LEAF,
              glm::vec3(position.x, position.y + 5.0f, position.z), 0),
   bRect(BoundingRectangle(glm::vec2(position.x, position.z), 
            glm::vec2(BOUNDING_SIZE, BOUNDING_SIZE), 0.0f)),
   rotate_(0.0f),
   elapsed_time_(0),
   rustle_time_(200),
   kMaxRustleTime(200),
   material_(glm::vec3(((rand() % 100) - 50) / 100.f * 0.9f)),
   translate_scale_(
      glm::translate(
         glm::mat4(),
         glm::vec3(position.x, position.y - 10.0f, position.z)
      ) *
      glm::scale(
         glm::mat4(),
         glm::vec3(TREE_SCALE) * glm::vec3(widthOffset, heightOffset, widthOffset)
      )
   ),
   default_model_(
      translate_scale_ *
      glm::rotate(
         glm::mat4(),
         angleRot,
         glm::vec3(0, 1.0f, 0)
      ) *
      glm::translate (
         glm::mat4(),
         glm::vec3(BOUNDING_ERR_X, 0.0f, BOUNDING_ERR_Z)
      ) *
      glm::rotate(
         glm::mat4(),
         -90.0f,
         glm::vec3(1.0f, 0, 0)
      )
   )

{} 

DrawInstance Tree::draw_instance() const {
   if (rustle_time_ >= kMaxRustleTime)
      return DrawInstance(default_model_, material_);
   const glm::mat4 rotate(
      glm::rotate(
         glm::mat4(),
         rotate_,
         glm::vec3(0, 0, 1.0f)
      )
   );

   return DrawInstance(default_model_ * rotate, material_);
}

bool Tree::isBlocker() {
   return true;
}

void Tree::performObjectHit(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::TREE_HIT,
         false,
         glm::vec3()
   );
   rustle_time_ = 0;
   leaf_system_.add();
}

void Tree::step(units::MS dt) {
   if (rustle_time_ < kMaxRustleTime) {
      rustle_time_ += dt;
      elapsed_time_ += dt;
      rotate_ = 10 * glm::sin(elapsed_time_ / 60.0f);
   }
   leaf_system_.step(dt);
}

