#include "song_path.h"

#include "sound_engine.h"

void SongPath::step(const BoundingRectangle& deer_rect) {
   if (song_stones_[current_stone_].bounding_rectangle().collidesWith(deer_rect)) {
      sound_engine_.playMusic();
   } else {
      sound_engine_.pauseMusic();
   }
}
