#include "game.h"
#include "graphics/mesh.h"
#include "graphics/shader_setup.h"
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/material.h>
#include <iostream>

#include "graphics/texture.h"
#include "DeerCam.h"
#include "AirCam.h"

namespace {
   DeerCam deerCam;
   AirCam airCam;
   bool showTreeShadows = false;
   bool draw_collision_box = false;
   bool debug = false;

   int lighting = 0;
   int raining = 0;

   float countLightning = 0.0;
   int numLightning = 0;
   Camera *curCam;
}

Game::Game() :
   attribute_location_map_({draw_shader_.getShaders().getAttributeLocationMap()}),
   ground_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::GROUND))),
   deer_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::DEER)), glm::vec3(0.0f)),
   day_night_boxes_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::TIME_STONE)), ground_),
   treeGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::TREE))),
   bushGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::BUSH))),
   flowerGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::FLOWER))),
   cardinal_bird_sound_(SoundEngine::SoundEffect::CARDINAL_BIRD, 10000),
   canary_bird_sound_(SoundEngine::SoundEffect::CANARY0, 4000),
   canary2_bird_sound_(SoundEngine::SoundEffect::CANARY1, 7000),
   woodpecker_bird_sound_(SoundEngine::SoundEffect::WOODPECKER0, 3000),
   butterfly_system_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::BUTTERFLY)), glm::vec3(0.0f), 10),
   rain_system_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::RAIN)),
            glm::vec3(0.0f, 100.0f, 0.0f), 2000),
   airMode(false),
   shadow_map_fbo_(kScreenWidth, kScreenHeight, SHADOW_MAP_TEXTURE, FBOType::DEPTH),
   water_(Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh(MeshType::GROUND)))
{

   std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
   std::cout << "Shader version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
   glClearColor (0.05098, 
                 0.6274509,
                 1.0,  1.0f);
   glClearDepth(1.0f);
   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);// Enable Depth Testing
   glDisable(GL_LIGHTING);
   glShadeModel(GL_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glEnable(GL_CULL_FACE);

   glPolygonMode(GL_FRONT, GL_LINE);
   glLineWidth(1.0);

   BoundingRectangle::loadBoundingMesh(mesh_loader_, attribute_location_map_);
   deerCam.initialize(deer_.getPosition());
   airCam.initialize(deer_.getPosition());

   treeGen.generate();
   bushGen.generate(ground_);
   flowerGen.generate();

   std::vector<GameObject*> objects;

   for (auto& tree : treeGen.getTrees()) {
      objects.push_back(&tree);
   }
   for (auto& bush : bushGen.getBushes()) {
      objects.push_back(&bush);
   }
   for (auto& flower : flowerGen.getFlowers()) {
      objects.push_back(&flower);
   }

   //Pre-processing BVH Tree
   objTree.calculateTree(objects);
   //objTree.printTree();
}

void Game::step(units::MS dt) {
   cardinal_bird_sound_.step(dt, sound_engine_);
   canary_bird_sound_.step(dt, sound_engine_);
   canary2_bird_sound_.step(dt, sound_engine_);
   woodpecker_bird_sound_.step(dt, sound_engine_);
   bool deerBlocked = false;
   

   if (airMode) {
      curCam = &airCam;
   }
   else {
      curCam = &deerCam;
   }

   sound_engine_.set_listener_position(deer_.getPosition(), deer_.getFacing());
   butterfly_system_.step(dt);
   rain_system_.step(dt);

   if(numLightning) {
      countLightning += dt/100.0;
      if(countLightning < 0.0)
         lighting = 0;
      else if(countLightning >= 0.0 && countLightning <= 1.0)
         lighting = 1;
      else if(countLightning > 1.0) {
         countLightning = -2.0;
         numLightning--;
         if(numLightning == 1)
            countLightning = -7.5;
      }
   }
   else
      lighting = 0;

   if (deer_.isMoving()) {
      BoundingRectangle nextDeerRect = deer_.getNextBoundingBox(dt, *curCam);
      std::vector<GameObject*> collObjs = objTree.getCollidingObjects(nextDeerRect);
      for (int index = 0; index < (int)(collObjs.size()); index++) {
         collObjs.at(index)->performObjectHit(sound_engine_);
         deerBlocked = deerBlocked || collObjs.at(index)->isBlocker();
      }

      glm::vec2 center = nextDeerRect.getCenter();

      deerBlocked = deerBlocked || center.x > GroundPlane::GROUND_SCALE / 2 || center.y > GroundPlane::GROUND_SCALE / 2 || center.x < -GroundPlane::GROUND_SCALE / 2 || center.y < -GroundPlane::GROUND_SCALE / 2;

      //printf("Next deer rect at (%f, %f) with dim (%f, %f)\n", center.x, center.y, nextDeerRect.getDimensions().x, nextDeerRect.getDimensions().y);
      deerCam.move(deer_.getPosition());
      airCam.move(deer_.getPosition());
   }

   if (deerBlocked) {
      deer_.block();
   }
   else {
      deer_.step(dt, *curCam, ground_, sound_engine_);
   }

   for (auto& bush : bushGen.getBushes()) {
      bush.step(dt);
   }

   if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_moon())) {
      day_cycle_.dayToNight();
   }
   else if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_sun())) {
      day_cycle_.nightToDay();
   }

   day_cycle_.autoAdjustTime(dt);
}

void Game::draw() {
   float sunIntensity = day_cycle_.getSunIntensity();
   glm::vec3 sunDir = day_cycle_.getSunDir();
   glm::vec3 deerPos;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor (0.05098 * sunIntensity,
                 0.6274509 * sunIntensity,
                 sunIntensity, 1.0f);

   glm::mat4 viewMatrix = deerCam.getViewMatrix();
   std::vector<Drawable> drawables;
   std::vector<CulledDrawable> culledDrawables;
   Drawable br_drawable;
   br_drawable.draw_template = BoundingRectangle::draw_template();
 
   drawables.push_back(deer_.drawable());
   br_drawable.model_transforms.push_back(deer_.bounding_rectangle().model_matrix());
  
   drawables.push_back(day_night_boxes_.drawableSun());
   br_drawable.model_transforms.push_back(day_night_boxes_.bounding_rectangle_sun().model_matrix());
  
   drawables.push_back(day_night_boxes_.drawableMoon());
   br_drawable.model_transforms.push_back(day_night_boxes_.bounding_rectangle_moon().model_matrix());
   
   drawables.push_back(bushGen.drawable());
   for (auto& bush : bushGen.getBushes()) {
      br_drawable.model_transforms.push_back(bush.getBoundingRectangle().model_matrix());
   }
  
   drawables.push_back(treeGen.drawable());
   for (auto& tree : treeGen.getTrees()) {
      br_drawable.model_transforms.push_back(tree.getBoundingRectangle().model_matrix());
   }
  
   drawables.push_back(flowerGen.drawable());
   for (auto& flower : flowerGen.getFlowers()) {
      br_drawable.model_transforms.push_back(flower.getBoundingRectangle().model_matrix());
   }
   
   if(raining)
      drawables.push_back(rain_system_.drawable());
   
   drawables.push_back(butterfly_system_.drawable());
   
   drawables.push_back(ground_.drawable());
   drawables.push_back(water_.drawable());
   if (draw_collision_box)
      drawables.push_back(br_drawable);

   viewMatrix = airMode ? airCam.getViewMatrix() : deerCam.getViewMatrix();
   deerPos = deer_.getPosition();

   // Iterate through drawables
   //create spheres
   //test if in VF

   FrustumG viewFrust;
   int culledObject = 0;

   viewFrust.setCamInternals(10.0, 1.0, 1.0, 5.0);
   viewFrust.setCamDef(curCam->getPosition(),
                       curCam->getLookAt(),
                       curCam->up);

   for (auto& drawable : drawables) {
      glm::vec3 min, max, med;

      CulledDrawable culledDrawable;
      CulledTransform culledTransform;
      for (auto& transform : drawable.model_transforms) {
         min = glm::vec3(transform * glm::vec4(drawable.draw_template.mesh.min, 0));
         max = glm::vec3(transform * glm::vec4(drawable.draw_template.mesh.max, 0));
         med = glm::vec3((min.x + max.x) / 2,
                         (min.y + max.y) / 2,
                         (min.z + max.z) / 2);
         culledTransform.model = transform;
         std::cout << "in sphere: " << viewFrust.sphereInFrustum(med, dist(max, med)) << std::endl;
         if (!viewFrust.sphereInFrustum(med, dist(max, med))) {
            culledTransform.cullFlag.insert(CullType::VIEW_CULLING);
            culledObject++;
         }
         // Test for reflection
         if (!viewFrust.sphereInFrustum(med, dist(max, med))) {
            culledTransform.cullFlag.insert(CullType::REFLECT_CULLING);
         }
         culledDrawable.model_transforms.push_back(culledTransform);
      }
      culledDrawables.push_back(culledDrawable);
   }
   std::cout << "culledObjects: " << culledObject << std::endl;

   draw_shader_.Draw(shadow_map_fbo_, water_.fbo(), culledDrawables, viewMatrix, deerPos, sunDir, sunIntensity, lighting);
}

void Game::mainLoop() {
   Input input;
   showTreeShadows = false;
   bool running = true;
   SDL_Event event;
   units::MS previous_time = SDL_GetTicks();

   while (running) {
      {  // Collect input
         input.beginFrame();
         while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
               running = false;
            }

            if (event.type == SDL_KEYDOWN && event.key.repeat == false) {
               input.keyDown(event.key);
            } else if (event.type == SDL_KEYUP) {
               input.keyUp(event.key);
            }
         }
      }
      { // Handle input
         if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            running = false;
         }
         { // handle walk forward/backward for deer.
            const auto key_forward = SDL_SCANCODE_W;
            const auto key_backward = SDL_SCANCODE_S;
            if (input.isKeyHeld(key_forward) && !input.isKeyHeld(key_backward)) {
               deer_.walkForward();
            } else if (!input.isKeyHeld(key_forward) && input.isKeyHeld(key_backward)) {
               deer_.walkBackward();
            } else {
               deer_.stopWalking();
            }
         }
         { // handle strafe left/right for deer.
            const auto key_left = SDL_SCANCODE_A;
            const auto key_right = SDL_SCANCODE_D;
            if (input.isKeyHeld(key_left) && !input.isKeyHeld(key_right)) {
               deer_.strafeLeft();
               deerCam.rotatePositionWithDrag(-20, 0, kScreenWidth, kScreenHeight);
            } else if (!input.isKeyHeld(key_left) && input.isKeyHeld(key_right)) {
               deer_.strafeRight();
               deerCam.rotatePositionWithDrag(20, 0, kScreenWidth, kScreenHeight);
            } else {
               deer_.stopStrafing();
            }
         }
         { // handle jumping
            const auto key_jump = SDL_SCANCODE_J;
            if (input.wasKeyPressed(key_jump)) {
               deer_.jump();
            }
         }
         { // show or hide tree shadows -- Katelyn
            const auto key_tree = SDL_SCANCODE_T;
            if (input.wasKeyPressed(key_tree)) {
               showTreeShadows = !showTreeShadows;
               treeGen.includeInShadows(showTreeShadows); 
            }
         }
         { //handle debug for Katelyn
            const auto key_quit = SDL_SCANCODE_1;
            if (input.wasKeyPressed(key_quit)) {
               debug = !debug;
            }
         }
         {
            const auto key_toNight = SDL_SCANCODE_3;
            if (input.wasKeyPressed(key_toNight)) {
               day_cycle_.dayToNight();
            }
         }
         {
            const auto key_toDay = SDL_SCANCODE_4;
            if (input.wasKeyPressed(key_toDay)) {
               day_cycle_.nightToDay();
            }
         }
         { // Lightning
            const auto key_lightning = SDL_SCANCODE_L;
            if (input.wasKeyPressed(key_lightning)) {
               lighting = 1;
               numLightning = 3;
            }
         }
         { // Rain
            const auto key_rain = SDL_SCANCODE_R;
            if (input.wasKeyPressed(key_rain)) {
               raining = !raining;
            }
         }
         { //handle toggle between cameras
            const auto key_air_mode = SDL_SCANCODE_V;
            if (input.wasKeyPressed(key_air_mode)) {
               airMode = !airMode;
            }
         }
         { //handle quit
            const auto key_quit = SDL_SCANCODE_Q;
            if (input.wasKeyPressed(key_quit)) {
               running = false;
            }
         }
      }

      {
         const units::MS current_time = SDL_GetTicks();
         const units::MS dt = current_time - previous_time;
         step(dt);
         previous_time = current_time;
      }

      {
         draw();
         engine_.swapWindow();
      }

      SDL_Delay(5);
   }
}
