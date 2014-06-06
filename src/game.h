#ifndef GAME_H_
#define GAME_H_

#include "graphics/draw_shaders.h"
#include "graphics/assimp/mesh_loader.h"
#include "deer.h"
#include "input.h"
#include "sdl_engine.h"
#include "units.h"
#include "ground_plane.h"
#include "graphics/material.h"
#include "TreeGenerator.h"
#include "BushGenerator.h"
#include "FlowerGenerator.h"
#include "lightning_interaction.h"
#include "BVHTree.h"
#include "graphics/gl_adapters/frame_buffer_object.h"
#include "graphics/gl_adapters/deferred_frame_buffer.h"
#include "sound_engine.h"
#include "bird_sound.h"
#include "day_night_interaction.h"
#include "butterfly_system.h"
#include "rain_system.h"
#include "leaf_system.h"
#include "water.h"
#include "song_path.h"
#include "dist.h"
#include "god_rays.h"
#include "Skybox.h"
#include "Camera.h"
#include "pinecone.h"
#include "RockGenerator.h"

struct Game {
   Game();

   void mainLoop();

  private:
   SDLEngine engine_;
   Input input_;
   DrawShader draw_shader_;
   MeshLoader mesh_loader_;
   AttributeLocationMap attribute_location_map_;
   GroundPlane ground_;
   Deer deer_;
   DayNightInteraction day_night_boxes_;
   DayCycle day_cycle_;

   TreeGenerator treeGen; //May want this in a world generator class later
   BushGenerator bushGen;
   RockGenerator rockGen;

   /*temporary solution for two textures*/
   FlowerGenerator daisyGen;
   FlowerGenerator roseGen;

   SoundEngine sound_engine_;
   BirdSound cardinal_bird_sound_, canary_bird_sound_, canary2_bird_sound_, woodpecker_bird_sound_;

   /*temporary solution for three textures*/
   ButterflySystem butterfly_system_red_;
   ButterflySystem butterfly_system_pink_;
   ButterflySystem butterfly_system_blue_;

   RainSystem rain_system_;
   Lightning lightning_trigger_;
   GodRays god_rays_; 

   BVHTree objTree;

   Skybox skybox;

   Camera deerCam;

   FrameBufferObject deferred_diffuse_fbo_;
   FrameBufferObject deferred_position_fbo_;
   FrameBufferObject deferred_normal_fbo_;

   FrameBufferObject shadow_map_fbo_;
   Water water_;
   SongPath song_path_;
   Mesh screen_plane_mesh_;

   Pinecone pinecone_;

   void step(units::MS dt);
   void draw();
};

#endif // GAME_H_
