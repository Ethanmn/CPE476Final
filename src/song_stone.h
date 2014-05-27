#ifndef SONG_STONE_H_
#define SONG_STONE_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"

struct SongStone {
   SongStone(const glm::vec2& position);

   void step(units::MS dt);
   void reset() { life_time_ = 0; }
   bool expired() const;
   glm::mat4 model() const;
   BoundingRectangle bounding_rectangle() const {
      return bounding_rectangle_;
   }

  private:
   units::MS life_time_;
   const glm::vec2 position_;
   BoundingRectangle bounding_rectangle_;
};

#endif // SONG_STONE_H_
