#ifndef SONG_PATH_H_
#define SONG_PATH_H_

#include <vector>

#include "song_stone.h"

struct SoundEngine;

struct SongPath {
   SongPath(SoundEngine& sound_engine, const Mesh& mesh) :
      current_stone_(0),
      song_stones_({
            SongStone()
            }),
      sound_engine_(sound_engine),
      mesh_(mesh)
   {}

   void step(const BoundingRectangle& deer_rect);
   std::vector<BoundingRectangle> bounding_rectangles() const {
      std::vector<BoundingRectangle> brs;
      for (auto& s : song_stones_) {
         brs.push_back(s.bounding_rectangle());
      }
      return brs;
   }

   Drawable drawable() const {
      std::vector<glm::mat4> models;
      for (const auto& stone : song_stones_) {
         models.push_back(stone.model());
      }
      return Drawable({
            DrawTemplate({
               ShaderType::TEXTURE,
               mesh_,
               boost::none,
               boost::none,
               EffectSet(),
            }),
            models
            });
   }

  private:
   size_t current_stone_;

   std::vector<SongStone> song_stones_;
   SoundEngine& sound_engine_;
   Mesh mesh_;
};

#endif // SONG_PATH_H_
