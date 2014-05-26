#ifndef FLOWER_GENERATOR_H_
#define FLOWER_GENERATOR_H_

#include "ground_plane.h"
#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "bounding_rectangle.h"
#include "Flower.h"
#include "graphics/draw_template.h"

struct FlowerGenerator {
   FlowerGenerator(const Mesh& mesh);

   void generate(const GroundPlane& ground);
   std::vector<Flower>& getFlowers();

   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;
   
   private:
      std::vector<Flower> flowers;
      DrawTemplate draw_template_;
};


#endif
