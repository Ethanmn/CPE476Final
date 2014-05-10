/*
   Tree.h
   Katie Keim
   CPE 476 - Deer
*/
#ifndef TREE_H_
#define TREE_H_

#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "ground_plane.h"
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "graphics/shader_setup.h"

struct Tree : public GameObject {
   Tree(int x, int z, Mesh mesh);

   void draw(Shader& shader, const UniformLocationMap& uniform_location_map, const glm::mat4& view_matrix) const;
   BoundingRectangle getBoundingRectangle();
   bool isBlocker();
   void performObjectHit();

   private:
      BoundingRectangle bRect;
      glm::vec2 position;
      Mesh mesh;
};

#endif //TREE_H_
