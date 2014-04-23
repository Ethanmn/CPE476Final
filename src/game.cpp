#include "game.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace {
   DeerCam deerCam;
   glm::vec3 sunDir;
   float sunIntensity;
}

Game::Game() :
   texture_(texture_path(Textures::GRASS)),
   deer_texture_(texture_path(Textures::DEER)),
   attribute_location_map_(shaders_.getAttributeLocationMap()),
   uniform_location_map_(shaders_.getUniformLocationMap()),
   ground_(attribute_location_map_),
   deer_(Mesh::fromAssimpMesh(attribute_location_map_,
         loadMesh("../models/Test_Deer2.dae")), glm::vec3(0.0f)),
   tree_(Mesh::fromAssimpMesh(
            attribute_location_map_,
            loadMesh("../models/tree.3ds")),
         glm::vec3(30, 0, 25),
         1.2f)
{
   glClearColor(0, 0, 0, 1); // Clear to solid blue.
   glClearDepth(1.0f);
   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);// Enable Depth Testing
   glDisable(GL_LIGHTING);
   glShadeModel(GL_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glEnable(GL_CULL_FACE);

   glPolygonMode(GL_FRONT, GL_LINE);
   glLineWidth(1.0);

   BoundingRectangle::loadBoundingMesh(attribute_location_map_);
   mouseDown = false;
   deerCam.initialize(deer_.getPosition());
}

void Game::step(units::MS dt) {
   deer_.step(dt, deerCam);

   if (deer_.isMoving()) {
      deerCam.move(deer_.getPosition());
   }
}

void Game::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glm::mat4 viewMatrix, modelMatrix;
   modelMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f));

   for (auto& shaderPair: shaders_.getMap()) {
      Shader& shader = shaderPair.second;
      shader.use();

      shader.sendUniform(Uniform::MODEL, uniform_location_map_,
            modelMatrix);
      shader.sendUniform(Uniform::VIEW, uniform_location_map_,
            deerCam.getViewMatrix());
      shader.sendUniform(Uniform::PROJECTION, uniform_location_map_,
            glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 100.f));

      if(shaderPair.first == ShaderType::TEXTURE) {
         texture_.enable();

         shader.sendUniform(Uniform::TEXTURE, uniform_location_map_, 0);
         ground_.draw(shader, uniform_location_map_);

         /* Render deer with texture -- please don't delete, used for debugging shaders
            texture_.disable();
            texture_.enable();
            shader.sendUniform(Uniform::TEXTURE, uniform_location_map_, 0);
            deer_.draw(shader, uniform_location_map_);
            */
      }
      else if(shaderPair.first == ShaderType::SUN) {
         mat_.sendToShader(shader, uniform_location_map_);
         shader.sendUniform(Uniform::SUN_DIR, uniform_location_map_, sunDir);
         shader.sendUniform(Uniform::SUN_INTENSITY, uniform_location_map_, sunIntensity);

         shader.sendUniform(Uniform::NORMAL, uniform_location_map_, glm::transpose(glm::inverse(deerCam.getViewMatrix())));
         mat_.changeDiffuse(glm::vec3(0.7f, 0.5f, 0.7f), shader, uniform_location_map_);

         tree_.draw(shader, uniform_location_map_, deerCam.getViewMatrix());

         mat_.changeDiffuse(glm::vec3(0.45, 0.24, 0.15), shader, uniform_location_map_);
         deer_.draw(shader, uniform_location_map_, deerCam.getViewMatrix());

         /* Render ground as dark green -- please don't delete, used for debugging shaders
            mat_.changeDiffuse(glm::vec3(0.051 * 1.5, 0.2431 * 1.5, 0.1568 * 1.5), shader, uniform_location_map_);
            ground_.draw(shader, uniform_location_map_);
            */

      }
      else if(shaderPair.first == ShaderType::WIREFRAME)
         shader.sendUniform(Uniform::COLOR, uniform_location_map_, glm::vec4(1, 0, 0, 1));

      if(shaderPair.first == ShaderType::TEXTURE)
         texture_.disable();
   }
}

void Game::mainLoop() {
   sunDir = glm::vec3(1.5, 1.0, 0.4);
   sunIntensity = 0.8;

   Input input;
   int mX, mY;
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
            else if (event.type == SDL_MOUSEBUTTONDOWN && SDL_GetMouseState(&mX, &mY)) {
               mousePos = glm::vec2(mX, mY);
               mouseDown = true;
            }
            else if (event.type == SDL_MOUSEMOTION && SDL_GetMouseState(&mX, &mY) && mouseDown) {
               deerCam.rotatePositionWithDrag(mousePos, glm::vec2(mX, mY), kScreenWidth, kScreenHeight);
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
               mouseDown = false;
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
