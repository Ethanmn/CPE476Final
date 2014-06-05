#ifndef PINECONE_H_
#define PINECONE_H_

#include "graphics/draw_template.h"

struct GroundPlane;

struct Pinecone {
   Pinecone(const Mesh& mesh, const GroundPlane& ground, const glm::vec2& position);

   Drawable drawable() const;

  private:
   DrawTemplate draw_template_;

   glm::vec3 position_;
};

#endif // PINECONE_H_
