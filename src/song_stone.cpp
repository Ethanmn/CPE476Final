#include "song_stone.h"

#include <glm/gtc/matrix_transform.hpp>

const units::MS kExpirationTime = 3000;

SongStone::SongStone(const glm::vec2& position) :
   life_time_(0),
   position_(position),
   bounding_rectangle_(position_, glm::vec2(30, 30), 0.0f)
{
}

void SongStone::step(units::MS dt) {
   if (life_time_ < kExpirationTime)
      life_time_ += dt;
}

bool SongStone::expired() const {
   return life_time_ >= kExpirationTime;
}

glm::vec2 SongStone::getPosition() const {
   return position_;
}

glm::mat4 SongStone::model() const {
   return glm::translate(glm::mat4(), glm::vec3(position_.x, 5, position_.y));
}

float SongStone::lifeRemainingRatio() {
   return (kExpirationTime - life_time_) / kExpirationTime;
}
