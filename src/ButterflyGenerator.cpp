/*
   ButterflyGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "ButterflyGenerator.h"

const int MAX_NUM_BUTTERFLIES = 20;
const int MIN_NUM_BUTTERFLIES = 5;

const int MAX_HEIGHT = 20;
const int MIN_HEIGHT = 10;

ButterflyGenerator::ButterflyGenerator(AttributeLocationMap attribute_location_map_, MeshLoader mesh_loader_) {
   systems.push_back(ButterflySystem(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::BUTTERFLY)), TextureType::BUTTERFLY_RED,
         glm::vec3(0.0f), 10));
   systems.push_back(ButterflySystem(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::BUTTERFLY)), TextureType::BUTTERFLY_PINK,
         glm::vec3(40.0f, 0.f, 50.0f), 10));
   systems.push_back(ButterflySystem(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::BUTTERFLY)), TextureType::BUTTERFLY_BLUE,
         glm::vec3(-60.0f, 0.f, -70.0f), 10));
}

ButterflySystem ButterflyGenerator::getSystem(glm::vec3 origin) {
   ButterflySystem system = systems.at(rand() % systems.size());
   float height = (rand() % (MAX_HEIGHT - MIN_HEIGHT)) + MIN_HEIGHT;

   system.generate(origin + glm::vec3(0, height, 0), (rand() % (MAX_NUM_BUTTERFLIES - MIN_NUM_BUTTERFLIES)) + MIN_NUM_BUTTERFLIES);

   return system;
}
