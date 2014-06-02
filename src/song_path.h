#ifndef SONG_PATH_H_
#define SONG_PATH_H_

#include <vector>

#include "song_stone.h"

struct SoundEngine;

struct SongPath {
   SongPath(SoundEngine& sound_engine, const Mesh& mesh);

   void step(units::MS dt, const BoundingRectangle& deer_rect);
   std::vector<BoundingRectangle> bounding_rectangles() const {
      std::vector<BoundingRectangle> brs;
      brs.push_back(song_stones_[current_stone_].bounding_rectangle());
      if (current_stone_ + 1 < song_stones_.size())
         brs.push_back(song_stones_[current_stone_ + 1].bounding_rectangle());
      return brs;
   }

   Drawable drawable() const {
      std::vector<glm::mat4> models;
      models.push_back(song_stones_[current_stone_].model());
      if (current_stone_ + 1 < song_stones_.size())
         models.push_back(song_stones_[current_stone_ + 1].model());
      return Drawable({
            DrawTemplate({
               ShaderType::TEXTURE,
               mesh_,
               Texture(TextureType::DEER, DIFFUSE_TEXTURE),
               boost::none,
               EffectSet()
            }),
            models
            });
   }

   glm::vec2 CurrentStonePosition();
   float CurrentStoneRemainingRatio();
   glm::vec2 NextStonePosition();

  private:
   size_t current_stone_;

   std::vector<SongStone> song_stones_;
   SoundEngine& sound_engine_;
   Mesh mesh_;
};

#endif // SONG_PATH_H_
