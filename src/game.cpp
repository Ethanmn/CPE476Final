#include "game.h"
#include "graphics/mesh.h"
#include "graphics/shader_setup.h"
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/material.h>
#include <iostream>

namespace {
   DeerCam deerCam;
   AirCam airCam;
}

Game::Game() :
   texture_(texture_path(Textures::GRASS)),
   attribute_location_map_(shaders_.getAttributeLocationMap()),
   uniform_location_map_(shaders_.getUniformLocationMap()),
   ground_(attribute_location_map_),
   deer_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh("../models/deer_butt.dae")), glm::vec3(0.0f)),
   day_night_boxes_(Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh("../models/cube.obj"))),
   treeGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh("../models/tree2.3ds"))),
   bushGen(Mesh::fromAssimpMesh(
            attribute_location_map_,
            mesh_loader_.loadMesh("../models/tree.3ds"))),
   objTree(),
   airMode(false)
{
   //glClearColor(0, 0, 0, 1); // Clear to solid blue.

   std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
   std::cout << "Shader version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
   glClearColor (0.05098 * 0.5, 0.6274509 * 0.5, 0.5, 1.0f);
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
   bushGen.generate();

   std::vector<GameObject*> objects;
   
   for (auto& tree : treeGen.getTrees()) {
      objects.push_back(&tree);
   }
   for (auto& bush : bushGen.getBushes()) {
      objects.push_back(&bush);
   }

   //Pre-processing BVH Tree
   objTree.calculateTree(objects);
   objTree.printTree();
}

void Game::step(units::MS dt) {
   bool deerBlocked = false;
   glm::vec3 dPos = deer_.getPosition();

   Camera *curCam;

   if (airMode) {
      curCam = &airCam;
   }
   else {
      curCam = &deerCam;
   }

   if (deer_.isMoving()) {
      BoundingRectangle nextDeerRect = deer_.getNextBoundingBox(dt, *curCam);
      std::vector<GameObject*> collObjs = objTree.getCollidingObjects(nextDeerRect);
      for (int index = 0; index < (int)(collObjs.size()); index++) {
         collObjs.at(index)->performObjectHit();
         deerBlocked = deerBlocked || collObjs.at(index)->isBlocker();
      }

      glm::vec2 center = nextDeerRect.getCenter();

      deerBlocked = deerBlocked || center.x > GroundPlane::GROUND_SCALE / 2 || center.y > GroundPlane::GROUND_SCALE / 2 || center.x < -GroundPlane::GROUND_SCALE / 2 || center.y < -GroundPlane::GROUND_SCALE / 2;

      //printf("Next deer rect at (%f, %f) with dim (%f, %f)\n", center.x, center.y, nextDeerRect.getDimensions().x, nextDeerRect.getDimensions().y);
   }

   if (deerBlocked) {
      deer_.block();
   }
   else {
      deer_.step(dt, *curCam);
   }

   

   for (auto& bush : bushGen.getBushes()) {
      bush.step(dt);
   }

   if (deer_.isMoving()) {
      deerCam.move(deer_.getPosition());
      airCam.move(deer_.getPosition());
   }

   if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_start())) {
      day_cycle_.on();
   }
   else if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_stop())) {
      day_cycle_.off();
   }

   day_cycle_.autoAdjustTime(dt);
}

void Game::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float sunIntensity = day_cycle_.getSunIntensity();
   glm::vec3 sunDir = day_cycle_.getSunDir();
   glm::mat4 viewMatrix; 
   glm::mat4 boxModelMatrix;

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

      if(shaderPair.first == ShaderType::TEXTURE) {
         setupView(shader, uniform_location_map_, viewMatrix);
         setupSunShader(shader, uniform_location_map_, sunIntensity, sunDir);
         setupTextureShader(shader, uniform_location_map_, sunIntensity, texture_.textureID());
         texture_.enable();
         ground_.draw(shader, uniform_location_map_, viewMatrix);
         texture_.disable();

         deer_.draw(shader, uniform_location_map_, viewMatrix, sunIntensity);

      }
      else if(shaderPair.first == ShaderType::SUN) {
         setupView(shader, uniform_location_map_, viewMatrix);
         setupSunShader(shader, uniform_location_map_, sunIntensity, sunDir);

         day_night_boxes_.drawStop(shader, uniform_location_map_, viewMatrix);
         day_night_boxes_.drawStart(shader, uniform_location_map_, viewMatrix);

         treeGen.drawTrees(shader, uniform_location_map_, viewMatrix);
         bushGen.drawBushes(shader, uniform_location_map_, viewMatrix);
      }
      else if(shaderPair.first == ShaderType::WIREFRAME)
         setupWireframeShader(shader, uniform_location_map_, glm::vec4(1, 0, 0, 1));
   }
}

void Game::mainLoop() {
   Input input;
   int mX, mY;
   bool running = true;
   SDL_Event event;
   units::MS previous_time = SDL_GetTicks();

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

            /*if (event.type == SDL_MOUSEMOTION) {
               SDL_GetRelativeMouseState(&mX, &mY);
               deerCam.rotatePositionWithDrag(mX, mY, kScreenWidth, kScreenHeight);
               mousePos = glm::vec2(mX, mY);
            }*/
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
               //day_cycle_.switchBoolean();
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
