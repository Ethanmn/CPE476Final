#include "game.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace {
   DeerCam deerCam;
   Mesh box;
   glm::vec3 deer_color;
}

Game::Game() :
   texture_(texture_path(Textures::GRASS)),
   deer_texture_(texture_path(Textures::DEER)),
   attribute_location_map_(shaders_.getAttributeLocationMap()),
   uniform_location_map_(shaders_.getUniformLocationMap()),
   ground_(attribute_location_map_),
   deer_(Mesh::fromAssimpMesh(attribute_location_map_,
            loadMesh("../models/Test_Deer2.dae")), glm::vec3(0.0f)),
   treeGen(Mesh::fromAssimpMesh(attribute_location_map_,
            loadMesh("../models/tree2.3ds"))),
   tree_mesh_(Mesh::fromAssimpMesh(
            attribute_location_map_,
            loadMesh("../models/tree.3ds"))),
   trees_{
      Tree(tree_mesh_,
            glm::vec3(30 - 15, 0, 25 + 5),
            1.2f,
            300),
      Tree(tree_mesh_,
            glm::vec3(20 - 15, 0, 18 + 5),
            0.8f,
            450),
      Tree(tree_mesh_,
            glm::vec3(25 - 15, 0, 12 + 5),
            0.9f,
            150),
      Tree(tree_mesh_,
            glm::vec3(12 - 15, 0, 24 + 5),
            1.3f,
            400),
   }
{
   //glClearColor(0, 0, 0, 1); // Clear to solid blue.

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

   BoundingRectangle::loadBoundingMesh(attribute_location_map_);
   mouseDown = false;
   deerCam.initialize(deer_.getPosition());
   treeGen.generateTrees();
}

void Game::step(units::MS dt) {
   bool treeColl = false;

   deer_.step(dt, deerCam);
   for (auto& tree : trees_) {
      tree.step(dt);
   }

   if (deer_.isMoving()) {
      deerCam.move(deer_.getPosition());
      for (auto& tree : trees_) {
         if (deer_.bounding_rectangle().collidesWith(tree.bounding_rectangle())) {
            tree.rustle();
         }
      }
   }

   for (auto& box : treeGen.getBoundingBoxes()) {
         treeColl = treeColl || deer_.bounding_rectangle().collidesWith(box);
   }

   if (treeColl) {
      //printf("Collided with tree!\n");
            deer_.jump();
	    deer_color = glm::vec3(0.225, 0.12, 0.075);
   }
   else {
      deer_color = glm::vec3(0.45, 0.24, 0.15);
   }
}

void Game::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glm::mat4 viewMatrix, modelMatrix;
   modelMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f));

   if(deer_.getPosition().x < -27.0 && deer_.getPosition().x > -32.0 &&
         deer_.getPosition().z < -27.0 && deer_.getPosition().z > -32.0) {
      day_cycle_.on();
   }
   if(deer_.getPosition().x > 17.0 && deer_.getPosition().x < 22.0 &&
         deer_.getPosition().z > 17.0 && deer_.getPosition().z < 22.0) {
      day_cycle_.off();
   }

   day_cycle_.autoAdjustTime();


   for (auto& shaderPair: shaders_.getMap()) {
      Shader& shader = shaderPair.second;
      shader.use();

      shader.sendUniform(Uniform::MODEL, uniform_location_map_,
            modelMatrix);
      shader.sendUniform(Uniform::VIEW, uniform_location_map_,
            deerCam.getViewMatrix());
      shader.sendUniform(Uniform::PROJECTION, uniform_location_map_,
            glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f));

      if(shaderPair.first == ShaderType::TEXTURE) {
         shader.sendUniform(Uniform::SUN_INTENSITY, uniform_location_map_, day_cycle_.getSunIntensity());

         texture_.enable(0);
         shader.sendUniform(Uniform::TEXTURE, uniform_location_map_, 0);
         ground_.draw(shader, uniform_location_map_);

         /*
            texture_.enable(1);
            shader.sendUniform(Uniform::TEXTURE, uniform_location_map_, 0);
            deer_.draw(shader, uniform_location_map_, deerCam.getViewMatrix());
            */

      }
      else if(shaderPair.first == ShaderType::SUN) {
         mat_.sendToShader(shader, uniform_location_map_);
         shader.sendUniform(Uniform::SUN_DIR, uniform_location_map_, day_cycle_.getSunDir());
         shader.sendUniform(Uniform::SUN_INTENSITY, uniform_location_map_, day_cycle_.getSunIntensity());

         shader.sendUniform(Uniform::NORMAL, uniform_location_map_, glm::transpose(glm::inverse(deerCam.getViewMatrix())));

         //ON BOX
         shader.sendUniform(Uniform::MODEL, uniform_location_map_,
               glm::translate(glm::mat4(1.0), glm::vec3(-30.0, -6.0, -30.0)));
         mat_.changeDiffuse(glm::vec3(0.6f, 0.9f, 0.6f), shader, uniform_location_map_);
         shader.drawMesh(box);

         //OFF BOX
         shader.sendUniform(Uniform::MODEL, uniform_location_map_,
               glm::translate(glm::mat4(1.0), glm::vec3(20.0, -6.0, 20.0)));
         mat_.changeDiffuse(glm::vec3(0.9f, 0.6f, 0.6f), shader, uniform_location_map_);
         shader.drawMesh(box);

         for (auto& tree : trees_) {
            tree.draw(shader, uniform_location_map_, deerCam.getViewMatrix());
         }

         mat_.changeDiffuse(deer_color, shader, uniform_location_map_);
         deer_.draw(shader, uniform_location_map_, deerCam.getViewMatrix());
	 mat_.changeDiffuse(glm::vec3(0.45, 0.24, 0.15), shader, uniform_location_map_);
         treeGen.drawTrees(shader, uniform_location_map_, deerCam.getViewMatrix());
      }
      else if(shaderPair.first == ShaderType::WIREFRAME)
         shader.sendUniform(Uniform::COLOR, uniform_location_map_, glm::vec4(1, 0, 0, 1));

      if(shaderPair.first == ShaderType::TEXTURE)
         texture_.disable();
   }
}

void Game::mainLoop() {
   box = Mesh::fromAssimpMesh(attribute_location_map_, loadMesh("../models/cube.obj"));
   deer_color = glm::vec3(0.45, 0.24, 0.15);
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
               //day_cycle_.switchBoolean();
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
