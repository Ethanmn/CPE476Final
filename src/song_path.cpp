#include "song_path.h"

#include "sound_engine.h"

SongPath::SongPath(SoundEngine& sound_engine, const Mesh& mesh) :
   current_stone_(0),
   song_stones_({
         SongStone(glm::vec2(-15, -15)),
         SongStone(glm::vec2(-30, -30)),
         SongStone(glm::vec2(-45, -45)),
         }),
   sound_engine_(sound_engine),
   mesh_(mesh) {}

void SongPath::step(units::MS dt, const BoundingRectangle& deer_rect) {
   if (!song_stones_[current_stone_].expired() &&
       song_stones_[current_stone_].bounding_rectangle().collidesWith(deer_rect)) {
      song_stones_[current_stone_].step(dt);
      sound_engine_.playMusic();
   } else if (current_stone_ + 1 < song_stones_.size() &&
              !song_stones_[current_stone_ + 1].expired() &&
              song_stones_[current_stone_ + 1].bounding_rectangle().collidesWith(deer_rect)) {
      song_stones_[++current_stone_].step(dt);
      sound_engine_.playMusic();
   } else {
      sound_engine_.pauseMusic();
   }
}
