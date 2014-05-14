#include "game.h"
#include "graphics/mesh.h"
#include "graphics/shader_setup.h"
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/material.h>
#include <iostream>

namespace {
   DeerCam deerCam;
   AirCam airCam;
   bool showTreeShadows = false;
   bool debug = false;
   bool betterShadows = false;

   int lighting = 0;
   float countLightning = 0.0;
   int numLightning = 0;
}

Game::Game() :
   attribute_location_map_(shaders_.getAttributeLocationMap()),
   uniform_location_map_(shaders_.getUniformLocationMap()),
   ground_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh("../models/ground_plane.obj"))),
   deer_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh("../models/deer_walk.dae")), glm::vec3(0.0f)),
   day_night_boxes_(Mesh::fromAssimpMesh(attribute_location_map_, 
                     mesh_loader_.loadMesh("../models/time_stone.dae")), ground_),
   treeGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh("../models/tree2.3ds"))),
   bushGen(Mesh::fromAssimpMesh(
            attribute_location_map_,
            mesh_loader_.loadMesh("../models/tree.3ds"))),
   cardinal_bird_sound_(SoundEngine::SoundEffect::CARDINAL_BIRD, 10000),
   canary_bird_sound_(SoundEngine::SoundEffect::CANARY0, 4000),
   canary2_bird_sound_(SoundEngine::SoundEffect::CANARY1, 7000),
   woodpecker_bird_sound_(SoundEngine::SoundEffect::WOODPECKER0, 3000),
   butterfly_system_(glm::vec3(0.0f), 10, attribute_location_map_, mesh_loader_),
   rain_system_(glm::vec3(0.0f, 100.0f, 0.0f), 2000, attribute_location_map_, mesh_loader_),
   objTree(),
   airMode(false)
{
   std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
   std::cout << "Shader version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
   glClearColor (1.0, 1.0, 1.0, 1.0f);
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

   std::vector<GameObject*> objects;

   for (auto& tree : treeGen.getTrees()) {
      objects.push_back(&tree);
   }
   for (auto& bush : bushGen.getBushes()) {
      objects.push_back(&bush);
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
   Camera *curCam;

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

   if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_start())) {
      day_cycle_.dayToNight();
   }
   else if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_stop())) {
      day_cycle_.nightToDay();
   }

   day_cycle_.autoAdjustTime(dt);
}

void Game::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float sunIntensity = day_cycle_.getSunIntensity();
   if (sunIntensity < 0.3)
      sunIntensity = 0.3;
   glm::vec3 sunDir = day_cycle_.getSunDir();
   glm::mat4 viewMatrix = deerCam.getViewMatrix();
   glm::vec3 deerPos = deerCam.getPosition();

   if (airMode) {
      viewMatrix = airCam.getViewMatrix();
   }
   else {
      viewMatrix = deerCam.getViewMatrix();
   }

   for (auto& shaderPair: shaders_.getMap()) {
      Shader& shader = shaderPair.second;
      shader.use();

      setupProjection(shader, uniform_location_map_);

      if(shaderPair.first == ShaderType::SHADOW_TEX) {
         if(!debug) {
            shadow_map_fbo_.BindForWriting();
            glClear(GL_DEPTH_BUFFER_BIT);
         }
      }
      else if(shaderPair.first == ShaderType::SHADOW) {
         deer_.shadowDraw(shader, uniform_location_map_, sunDir, betterShadows);

         day_night_boxes_.shadowDrawRed(shader, uniform_location_map_, sunDir, deerPos, betterShadows);
         day_night_boxes_.shadowDrawGreen(shader, uniform_location_map_, sunDir, deerPos, betterShadows);


         if(showTreeShadows)
            treeGen.shadowDraw(shader, uniform_location_map_, sunDir, deerPos, betterShadows);
         for (auto& bush : bushGen.getBushes()) {
            bush.shadowDraw(shader, uniform_location_map_, sunDir, deerPos, betterShadows);
         }


         if(!debug) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shadow_map_fbo_.BindForReading();
            glClearColor (0.05098 * sunIntensity,
                       0.6274509 * sunIntensity,
                       sunIntensity, 1.0f);
         }
      }
      else if(!debug && shaderPair.first == ShaderType::TEXTURE) {
         shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniform_location_map_,
            shadow_map_fbo_.texture_id());
         if(betterShadows)
            sendBetterShadowInverseProjectionView(shader, uniform_location_map_, sunDir, deerPos);
         else
            sendShadowInverseProjectionView(shader, uniform_location_map_, sunDir, deerPos);

         shader.sendUniform(Uniform::LIGHTNING, uniform_location_map_, lighting);

         setupView(shader, uniform_location_map_, viewMatrix);
         setupSunShader(shader, uniform_location_map_, sunIntensity, sunDir);

         ground_.draw(shader, uniform_location_map_, viewMatrix);
         deer_.draw(shader, uniform_location_map_, viewMatrix);

         day_night_boxes_.drawStop(shader, uniform_location_map_, viewMatrix);
         day_night_boxes_.drawStart(shader, uniform_location_map_, viewMatrix);

         butterfly_system_.draw(shader, uniform_location_map_, viewMatrix);
      }
      else if(!debug && shaderPair.first == ShaderType::SUN) {
         setupView(shader, uniform_location_map_, viewMatrix);
         setupSunShader(shader, uniform_location_map_, sunIntensity, sunDir);

         rain_system_.draw(shader, uniform_location_map_, viewMatrix);

         shader.sendUniform(Uniform::LIGHTNING, uniform_location_map_, lighting);
         treeGen.drawTrees(shader, uniform_location_map_, viewMatrix);
         bushGen.drawBushes(shader, uniform_location_map_, viewMatrix);
      }

      // If pixel is under ground draw as blue (water)?
   }
}

void Game::mainLoop() {
   Input input;
   showTreeShadows = false;
   bool running = true;
   SDL_Event event;
   units::MS previous_time = SDL_GetTicks();

   if(!debug && !shadow_map_fbo_.setup(kScreenWidth, kScreenHeight)) {
      printf("FAILURE\n");
      return;
   }

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
            }
         }
         { //handle debug for Katelyn
            const auto key_quit = SDL_SCANCODE_1;
            if (input.wasKeyPressed(key_quit)) {
               debug = !debug;
               glClearColor (1.0, 1.0, 1.0, 1.0f);
            }
         }
         { //handle debug for Katelyn
            const auto key_quit = SDL_SCANCODE_2;
            if (input.wasKeyPressed(key_quit)) {
               betterShadows = !betterShadows;
            }
         }
         { //handle debug for Katelyn
            const auto key_quit = SDL_SCANCODE_3;
            if (input.wasKeyPressed(key_quit)) {
               day_cycle_.dayToNight();
            }
         }
         { //handle debug for Katelyn
            const auto key_quit = SDL_SCANCODE_4;
            if (input.wasKeyPressed(key_quit)) {
               day_cycle_.nightToDay();
            }
         }
         { //handle quit
            const auto key_quit = SDL_SCANCODE_L;
            if (input.wasKeyPressed(key_quit)) {
               lighting = 1;
               numLightning = 3;
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
