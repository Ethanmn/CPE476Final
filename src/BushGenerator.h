/* 
   BushGenerator.h
   Katie Keim
   Deer - CPE 476
*/
#ifndef BUSH_GEN_H_
#define BUSH_GEN_H_

#include "ground_plane.h"
#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "bounding_rectangle.h"
#include "Bush.h"
#include "graphics/draw_template.h"
#include "butterfly_system.h"

struct BushGenerator {
   BushGenerator(const Mesh& mesh, const GroundPlane& ground, const Mesh& butterfly);

   std::vector<Bush>& getBushes();

   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;
   Drawable butterflyDrawable() const;

  private:
   void generate(const GroundPlane& ground);
   Mesh butterfly_;
   std::vector<Bush> bushes;
   DrawTemplate draw_template_;
};

#endif //BUSH_GEN_H_
