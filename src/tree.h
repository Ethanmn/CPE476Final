#ifndef TREE_H_
#define TREE_H_

#include <glm/glm.hpp>

#include "graphics/mesh.h"

struct Shader;

struct Tree {
   Tree(const Mesh& mesh, const glm::vec3& position) :
      position_(position),
      mesh_(mesh) {}

   void draw(
         Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix) const;

  private:
   glm::vec3 position_;
   Mesh mesh_;
};

#endif // TREE_H_
