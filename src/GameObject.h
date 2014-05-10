/*
   GameObject.h
   Katie Keim
   CPE 476 - Deer
*/
#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "bounding_rectangle.h"

struct GameObject {
   virtual ~GameObject(){}

   virtual BoundingRectangle getBoundingRectangle() {
      return BoundingRectangle(glm::vec2(0, 0), glm::vec2(0, 0), 0.0f);
   }

   virtual bool isBlocker() {
      return false;
   }

   virtual void performObjectHit() {}

   virtual void draw(Shader& shader, const UniformLocationMap& uniform_location_map, const glm::mat4& view_matrix) const {} 
};
#endif //GAME_OBJECT_H_
