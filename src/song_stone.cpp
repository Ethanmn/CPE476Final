#include "song_stone.h"

#include <glm/gtc/matrix_transform.hpp>

SongStone::SongStone() :
   position_(),
   bounding_rectangle_(position_, glm::vec2(30, 30), 0.0f)
{
}

glm::mat4 SongStone::model() const {
   return glm::translate(glm::mat4(), glm::vec3(0, 5, 0));
}
