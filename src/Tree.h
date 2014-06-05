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
#include "sound_engine.h"
#include "leaf_system.h"


struct Tree : public GameObject {
   Tree(glm::vec3 position, float heightOffset, float angleRot, const Mesh& leaf);

   BoundingRectangle getBoundingRectangle() {
      return bRect;
   }

   bool isBlocker();
   void performObjectHit(SoundEngine& sound_engine);

   glm::mat4 calculateModel() const;

   void step(units::MS dt);

   public:
      LeafSystem leaf_system_;

   private:
      BoundingRectangle bRect;
      glm::vec3 offset;
      float angleRot;
      glm::vec3 colorOffset;
      float heightOffset;

      float rotate_;
      units::MS elapsed_time_, rustle_time_;
      const units::MS kMaxRustleTime;
      glm::mat4 translate_scale_;
      glm::mat4 default_model_;
      MeshLoader mesh_loader_;
      AttributeLocationMap attribute_location_map_;
};

#endif //TREE_H_
