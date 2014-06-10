#ifndef PINECONE_H_
#define PINECONE_H_

#include "graphics/draw_template.h"
#include "bounding_rectangle.h"

struct GroundPlane;

struct Pinecone {
   Pinecone(const Mesh& mesh, const GroundPlane& ground, const glm::vec2& position);

   Drawable drawable() const;
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }
   BoundingRectangle aoe_bounding_rectangle() const { return aoe_bounding_rectangle_; }

   void kick(const glm::vec2& direction);
   void deer_pounces() { been_pounced_ = true; }
   bool been_pounced() const { return been_pounced_; }
   void step(float dt, const GroundPlane& ground);

  private:
   DrawTemplate draw_template_;
   BoundingRectangle bounding_rectangle_;
   BoundingRectangle aoe_bounding_rectangle_;

   glm::vec3 position_;
   glm::vec2 velocity_;
   bool been_pounced_;
};

#endif // PINECONE_H_
