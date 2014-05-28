/*
   Tree.cpp
   Katie Keim
   CPE 476 - Deer
   */
#include "Tree.h"

const int TREE_SCALE = 3;

const int BOUNDING_SIZE = 4;
const float BOUNDING_ERR_X = -0.5;
const float BOUNDING_ERR_Z = -3;

Tree::Tree(glm::vec3 position) :
   bRect(BoundingRectangle(glm::vec2(position.x + BOUNDING_ERR_X, position.z + BOUNDING_ERR_Z), 
            glm::vec2(BOUNDING_SIZE, BOUNDING_SIZE), 0.0f)),
   position(position.x, position.y - 10.0f, position.z),
   rotate_(0.0f),
   elapsed_time_(0),
   rustle_time_(200),
   kMaxRustleTime(200)
{}

glm::mat4 Tree::calculateModel() const {
   const auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(TREE_SCALE));

   const auto translate = glm::translate(
         glm::mat4(1.0f),
         glm::vec3(position.x, position.y, position.z));

   const glm::mat4 rotate(glm::rotate(
            glm::rotate(
               glm::mat4(),
               rotate_,
               glm::vec3(0, 1.0f, 0)
               ),
            -90.0f,
            glm::vec3(1, 0, 0)));

   return glm::mat4(translate * scale * rotate);// * rotateTreeUp);
} 

bool Tree::isBlocker() {
   return true;
}

void Tree::performObjectHit(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::TREE_HIT,
         false,
         position);
   rustle_time_ = 0;  
}

void Tree::step(units::MS dt) {
   if (rustle_time_ < kMaxRustleTime) {
      rustle_time_ += dt;
      elapsed_time_ += dt;
      rotate_ = 10 * glm::sin(elapsed_time_ / 60.0f);
   }
}

