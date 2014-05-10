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

struct BushGenerator {
   BushGenerator(const Mesh& mesh);

   void generate();
   std::vector<Bush> getBushes();

   private:
      std::vector<Bush> bushes;
      Mesh bushMesh1;
};

#endif //BUSH_GEN_H_
