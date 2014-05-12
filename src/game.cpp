#include "game.h"
#include "graphics/mesh.h"
#include "graphics/shader_setup.h"
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/material.h>
#include <iostream>

namespace {
   DeerCam deerCam;
   bool showTreeShadows = false;
   bool debug = false;
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
            mesh_loader_.loadMesh("../models/deer_butt.dae")), glm::vec3(0.0f)),
   day_night_boxes_(Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh("../models/cube.obj")), ground_),
   treeGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh("../models/tree2.3ds"))),
   tree_mesh_(Mesh::fromAssimpMesh(
            attribute_location_map_,
            mesh_loader_.loadMesh("../models/tree.3ds"))),
   bushes_({
      Tree(tree_mesh_,
            glm::vec3(30 - 15, 0, 25 + 5),
            ground_,
            1.2f,
            300),
      Tree(tree_mesh_,
            glm::vec3(20 - 15, 0, 18 + 5),
            ground_,
            0.8f,
            450),
      Tree(tree_mesh_,
            glm::vec3(25 - 15, 0, 12 + 5),
            ground_,
            0.9f,
            150),
      Tree(tree_mesh_,
            glm::vec3(12 - 15, 0, 24 + 5),
            ground_,
            1.3f,
            400),
   })
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
   treeGen.generateTrees();
}

void Game::step(units::MS dt) {
   bool treeColl = false;

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
   

   deer_.step(dt, deerCam, ground_);
   for (auto& tree : bushes_) {
      tree.step(dt);
   }

   if (deer_.isMoving()) {
      deerCam.move(deer_.getPosition());
      for (auto& tree : bushes_) {
         if (deer_.bounding_rectangle().collidesWith(tree.bounding_rectangle())) {
            tree.rustle();
         }
      }
   }

   for (auto& box : treeGen.getBoundingBoxes()) {
      treeColl = treeColl || deer_.bounding_rectangle().collidesWith(box);
   }

   if (treeColl)
      deer_.jump();

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
   glm::vec3 sunDir = day_cycle_.getSunDir();
   glm::mat4 viewMatrix = deerCam.getViewMatrix();
   glm::vec3 deerPos = deerCam.getPosition();

   for (auto& shaderPair: shaders_.getMap()) {
      Shader& shader = shaderPair.second;
      shader.use();
      
      setupProjection(shader, uniform_location_map_);
      if(shaderPair.first == ShaderType::SHADOW_TEX) {
         if(!debug) {
            shadow_map_fbo_.BindForWriting();
            glClear(GL_DEPTH_BUFFER_BIT);
         }
         ground_.shadowDraw(shader, uniform_location_map_, sunDir, deerPos);
      }
      else if(shaderPair.first == ShaderType::SHADOW) {
         deer_.shadowDraw(shader, uniform_location_map_, sunDir);
         
         day_night_boxes_.shadowDrawRed(shader, uniform_location_map_, sunDir, deerPos);
         day_night_boxes_.shadowDrawGreen(shader, uniform_location_map_, sunDir, deerPos);

         
         if(showTreeShadows)
            treeGen.shadowDraw(shader, uniform_location_map_, sunDir, deerPos);
         for (auto& bush : bushes_) {
            bush.shadowDraw(shader, uniform_location_map_, sunDir, deerPos);
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
         shader.sendUniform(Uniform::LIGHTNING, uniform_location_map_, lighting);

         sendShadowInverseProjectionView(shader, uniform_location_map_, sunDir, deerPos);
         setupView(shader, uniform_location_map_, viewMatrix);
         setupSunShader(shader, uniform_location_map_, sunIntensity, sunDir);

         ground_.draw(shader, uniform_location_map_, viewMatrix);
         deer_.draw(shader, uniform_location_map_, viewMatrix);
      }
      else if(!debug && shaderPair.first == ShaderType::SUN) {

         shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniform_location_map_, 
            shadow_map_fbo_.texture_id()); 
         shader.sendUniform(Uniform::LIGHTNING, uniform_location_map_, lighting);

         sendShadowInverseProjectionView(shader, uniform_location_map_, sunDir, deerPos);
         setupView(shader, uniform_location_map_, viewMatrix);
         setupSunShader(shader, uniform_location_map_, sunIntensity, sunDir);

         day_night_boxes_.drawStop(shader, uniform_location_map_, viewMatrix);
         day_night_boxes_.drawStart(shader, uniform_location_map_, viewMatrix);

         
         for (auto& bush : bushes_) {
            bush.draw(shader, uniform_location_map_, viewMatrix);
         }
         treeGen.drawTrees(shader, uniform_location_map_, viewMatrix);
         
      }
      else if (!debug && shaderPair.first == ShaderType::WIREFRAME) {
         setupWireframeShader(shader, uniform_location_map_, glm::vec4(1, 0, 0, 1));
      }

      // If pixel is under ground draw as blue (water)?
   }
}

void Game::mainLoop() {
   Input input;
   showTreeShadows = false;
   int mX, mY;
   bool running = true;
   SDL_Event event;
   units::MS previous_time = SDL_GetTicks();


   if(!debug && !shadow_map_fbo_.setup(kScreenWidth, kScreenHeight)) {
      printf("FAILURE\n");
      return;
   }


   SDL_WarpMouseInWindow(NULL, kScreenWidth / 2, kScreenHeight / 2);
   mousePos = glm::vec2(kScreenWidth / 2, kScreenHeight / 2);

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

            if (event.type == SDL_MOUSEMOTION) {
               SDL_GetRelativeMouseState(&mX, &mY);
               deerCam.rotatePositionWithDrag(mX, mY, kScreenWidth, kScreenHeight);
               mousePos = glm::vec2(mX, mY);
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
            } else if (!input.isKeyHeld(key_left) && input.isKeyHeld(key_right)) {
               deer_.strafeRight();
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
               day_cycle_.dayToNight();
            }
         }
         { //handle debug for Katelyn
            const auto key_quit = SDL_SCANCODE_3;
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
