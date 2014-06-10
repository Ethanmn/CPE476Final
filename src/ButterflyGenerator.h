/*
   ButterflyGenerator.h
   Katie Keim
   Deer - CPE 476
*/
#ifndef BUTTERFLY_GENERATOR_H
#define BUTTERFLY_GENERATOR_H

#include "butterfly_system.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include <vector>

struct ButterflyGenerator {
   ButterflyGenerator(AttributeLocationMap attribute_location_map_, MeshLoader mesh_loader_);

   ButterflySystem getSystem(glm::vec3 origin);

   private:
      std::vector<ButterflySystem> systems;
};

#endif //BUTTERFLY_GENERATOR_H
