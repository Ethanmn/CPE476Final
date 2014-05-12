#ifndef GAME_H_
#define GAME_H_

#include <glm/glm.hpp>
#include <vector>

#include "graphics/assimp/mesh_loader.h"
#include "deer.h"
#include "input.h"
#include "sdl_engine.h"
#include "units.h"
#include "graphics/shaders.h"
#include "ground_plane.h"
#include "graphics/texture.h"
#include "DeerCam.h"
#include "graphics/material.h"
#include "TreeGenerator.h"
#include "graphics/day_cycle.h"
#include "graphics/shadow_map.h"
#include "tree.h"
#include "sound_engine.h"
#include "bird_sound.h"
#include "day_night_interaction.h"
#include "BVHNode.h"
#include "butterfly_system.h"
#include "rain_system.h"

struct Game {
   Game();

   void mainLoop();

  private:
   SDLEngine engine_;
   Input input_;
   Shaders shaders_;
   MeshLoader mesh_loader_;
   AttributeLocationMap attribute_location_map_;
   UniformLocationMap uniform_location_map_;
   GroundPlane ground_;
   Deer deer_;
   DayNightInteraction day_night_boxes_;
   TreeGenerator treeGen; //May want this in a world generator class later
   DayCycle day_cycle_;
   Mesh tree_mesh_;
   std::vector<Tree> bushes_;
   ShadowMapFBO shadow_map_fbo_;
   SoundEngine sound_engine_;
   BirdSound cardinal_bird_sound_, canary_bird_sound_, canary2_bird_sound_, woodpecker_bird_sound_;
   ButterflySystem butterfly_system_;
   RainSystem rain_system_;


   glm::vec2 mousePos;

   void step(units::MS dt);
   void draw();
};

#endif // GAME_H_
