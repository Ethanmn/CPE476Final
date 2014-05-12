/*
   GameObject.h
   Katie Keim
   CPE 476 - Deer
*/
#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "bounding_rectangle.h"

struct SoundEngine;

struct GameObject {
   virtual BoundingRectangle getBoundingRectangle() = 0;

   virtual bool isBlocker() = 0;

   virtual void performObjectHit(SoundEngine& sound_engine) = 0;

   virtual void draw(Shader& shader, const UniformLocationMap& uniform_location_map, const glm::mat4& view_matrix) const = 0; 
};
#endif //GAME_OBJECT_H_
