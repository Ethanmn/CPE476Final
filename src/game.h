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
#include "graphics/texture_2d.h"
//#include "graphics/texture_3d.h"
#include "DeerCam.h"
#include "graphics/material.h"
#include "TreeGenerator.h"
#include "graphics/day_cycle.h"
#include "tree.h"
#include "sound_engine.h"
#include "bird_sound.h"

struct Game {
   Game();

   void mainLoop();

  private:
   SDLEngine engine_;
   Input input_;
   Shaders shaders_;
   MeshLoader mesh_loader_;
   Texture2D texture_;
   AttributeLocationMap attribute_location_map_;
   UniformLocationMap uniform_location_map_;
   GroundPlane ground_;
   Deer deer_;
   TreeGenerator treeGen; //May want this in a world generator class later
   DayCycle day_cycle_;
   Mesh tree_mesh_;
   std::vector<Tree> bushes_;
   SoundEngine sound_engine_;
   BirdSound cardinal_bird_sound_, canary_bird_sound_, canary2_bird_sound_, woodpecker_bird_sound_;

   glm::vec2 mousePos;
   bool mouseDown;

   void step(units::MS dt);
   void draw();
   void moveMouse(int endX, int endY);
};

#endif // GAME_H_
