#ifndef SONG_STONE_H_
#define SONG_STONE_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"

struct SongStone {
   SongStone();

   glm::mat4 model() const;
   BoundingRectangle bounding_rectangle() const {
      return bounding_rectangle_;
   }

  private:
   const glm::vec2 position_;
   BoundingRectangle bounding_rectangle_;
};

#endif // SONG_STONE_H_
