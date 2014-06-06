/* 
   RockGenerator.h
   Katie Keim
   Deer - CPE 476
*/
#ifndef ROCK_GEN_H_
#define ROCK_GEN_H_

#include "ground_plane.h"
#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "bounding_rectangle.h"
#include "Rock.h"
#include "graphics/draw_template.h"

struct RockGenerator {
   RockGenerator(const Mesh& mesh, const GroundPlane& ground);

   std::vector<Rock>& getRocks();

   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;

  private:
   void generate(const GroundPlane& ground);

   std::vector<Rock> rocks;
   DrawTemplate draw_template_;
};

#endif //ROCK_GEN_H_
