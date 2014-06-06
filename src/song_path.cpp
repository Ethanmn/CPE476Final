#include "song_path.h"

#include "irrKlang.h"

namespace {
std::vector<SongStone> create_path() {
   std::vector<SongStone> stones;
   float radius = 200.0f;
   for (float t = 0; t < radius; t += 15.f) {
      stones.push_back(SongStone(glm::vec2(t, 0)));
   }
   for (float t = 0; t < 360.0f; t += 0.08f) {
      stones.push_back(SongStone(glm::vec2(glm::cos(t) * radius, glm::sin(t) * radius)));
   }
   return stones;
}
}

SongPath::SongPath(irrklang::ISound* song, const Mesh& mesh) :
   current_stone_(0),
   song_stones_(create_path()),
   song_(song),
   draw_template_({
               ShaderType::DEFERRED,
               mesh,
               Material(glm::vec3(1, 0, 0)),
               Texture(TextureType::GEM, DIFFUSE_TEXTURE),
               boost::none,
               EffectSet(),
            })
{
}

glm::vec2 SongPath::CurrentStonePosition() {
   return song_stones_[current_stone_].getPosition();
}

float SongPath::CurrentStoneRemainingRatio() {
   return song_stones_[current_stone_].lifeRemainingRatio();
}

glm::vec2 SongPath::NextStonePosition() {
   return song_stones_[current_stone_ + 1].getPosition();
}
   
void SongPath::set_song(irrklang::ISound* song) {
   song_->stop();
   song_ = song;
}

void SongPath::reset() {
   for (auto& s : song_stones_) {
      s.reset();
   }
   current_stone_ = 0;
}

void SongPath::step(units::MS dt, const BoundingRectangle& deer_rect) {
   if (!song_->getIsPaused()) {
      draw_template_.mesh.animation.step(dt);
      if (draw_template_.mesh.animation.is_finished()) {
         draw_template_.mesh.animation.reset();
      }
   }
   if (!song_stones_[current_stone_].expired() &&
       song_stones_[current_stone_].bounding_rectangle().collidesWith(deer_rect)) {
      song_stones_[current_stone_].step(dt);
      song_->setIsPaused(false);
   } else if (current_stone_ + 1 < song_stones_.size() &&
              !song_stones_[current_stone_ + 1].expired() &&
              song_stones_[current_stone_ + 1].bounding_rectangle().collidesWith(deer_rect)) {
      song_stones_[++current_stone_].step(dt);
      song_->setIsPaused(false);
   } else {
      song_->setIsPaused(true);
   }
}
