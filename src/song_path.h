#ifndef SONG_PATH_H_
#define SONG_PATH_H_

#include <vector>

#include "song_stone.h"

namespace irrklang {
class ISound;
}

struct SongPath {
   SongPath(irrklang::ISound* song, const Mesh& mesh);

   void set_song(irrklang::ISound* song);

   void step(units::MS dt, const BoundingRectangle& deer_rect);
   std::vector<BoundingRectangle> bounding_rectangles() const {
      std::vector<BoundingRectangle> brs;
      brs.push_back(song_stones_[current_stone_].bounding_rectangle());
      if (current_stone_ + 1 < song_stones_.size())
         brs.push_back(song_stones_[current_stone_ + 1].bounding_rectangle());
      return brs;
   }

   Drawable drawable() const {
      std::vector<DrawInstance> models;
      models.push_back(song_stones_[current_stone_].model());
      if (current_stone_ + 1 < song_stones_.size())
         models.push_back(song_stones_[current_stone_ + 1].model());
      return Drawable({
            draw_template_,
            models
            });
   }

   glm::vec2 CurrentStonePosition();
   float CurrentStoneRemainingRatio();
   glm::vec2 NextStonePosition();
   void reset();

  private:
   size_t current_stone_;

   std::vector<SongStone> song_stones_;
   irrklang::ISound* song_;
   DrawTemplate draw_template_;
};

#endif // SONG_PATH_H_
