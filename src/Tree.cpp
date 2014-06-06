/*
   Tree.cpp
   Katie Keim
   CPE 476 - Deer
   */
#include "Tree.h"
#include <iostream>

const int TREE_SCALE = 3;
const int TREE_VARIATION_HEIGHT = 10;

const int BOUNDING_SIZE = 4;
const float BOUNDING_ERR_X = 0.15f;
const float BOUNDING_ERR_Z = 1.0f;

Tree::Tree(glm::vec3 position, float heightOffset, float angleRot, const Mesh& leaf) :
   bRect(BoundingRectangle(glm::vec2(position.x, position.z), 
            glm::vec2(BOUNDING_SIZE, BOUNDING_SIZE), 0.0f)),
   heightOffset(heightOffset),
   angleRot(angleRot),
   leaf_system_(leaf, TextureType::LEAF,
              glm::vec3(position.x, position.y + 5.0f, position.z), 0),
   rotate_(0.0f),
   elapsed_time_(0),
   rustle_time_(200),
   kMaxRustleTime(200),
   translate_scale_(
      glm::translate(
         glm::mat4(),
         glm::vec3(position.x, position.y - 10.0f, position.z)
      ) *
      glm::scale(
         glm::mat4(),
         glm::vec3(TREE_SCALE + (rand() % 20)/20.0f) * glm::vec3(1, heightOffset, 1)
      )
   ),
   default_model_(
      translate_scale_ *
      glm::translate (
         glm::mat4(),
         glm::vec3(BOUNDING_ERR_X, 0.0f, BOUNDING_ERR_Z)
      ) *
      glm::rotate(
         glm::mat4(),
         -90.0f,
         glm::vec3(1, 0, 0)
      )
   )

{} 

glm::mat4 Tree::calculateModel() const {
   if (rustle_time_ >= kMaxRustleTime)
      return default_model_;
   const glm::mat4 rotate(glm::rotate(
            glm::rotate(
               glm::mat4(),
               rotate_,
               glm::vec3(0, 1.0f, 0)
               ),
            -90.0f,
            glm::vec3(1, 0, 0)));

   const glm::mat4 rotate2(
            glm::rotate(
               glm::mat4(),
               angleRot,
               glm::vec3(0, 1.0f, 0)
               ));

   const glm::mat4 translateToCenter = glm::translate(
            glm::mat4(),
            glm::vec3(BOUNDING_ERR_X, 0.0f, BOUNDING_ERR_Z));

   return glm::mat4(translate_scale_ * rotate2 * translateToCenter * rotate);
}

bool Tree::isBlocker() {
   return true;
}

void Tree::performObjectHit(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::TREE_HIT,
         false,
         glm::vec3());
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

