#include "song_path.h"

#include "sound_engine.h"

namespace {
std::vector<SongStone> create_path() {
   std::vector<SongStone> stones;
   float radius = 200.0f;
   for (float t = 0; t < 360.0f; t += 0.08f) {
      stones.push_back(
            SongStone(glm::vec2(glm::cos(t) * radius, glm::sin(t) * radius)));
   }
   return stones;
}
}

SongPath::SongPath(SoundEngine& sound_engine, const Mesh& mesh) :
   current_stone_(0),
   song_stones_(create_path()),
   sound_engine_(sound_engine),
   mesh_(mesh)
{
   mesh_.material.diffuse = glm::vec3(1, 0, 0);
}

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
