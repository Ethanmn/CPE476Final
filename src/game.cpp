#include "game.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/texture.h"
#include <iostream>

namespace {
   //Camera camera;
   Mesh bunny;
}

Game::Game() :
   ground_(shaders_) {
   glClearColor(0, 0, 0, 1); // Clear to solid blue.
   glClearDepth(1.0f);
   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);// Enable Depth Testing
   glDisable(GL_LIGHTING);
   glShadeModel(GL_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glEnable(GL_CULL_FACE);
   initTexture();
}

void Game::step(units::MS) {
   
}

void Game::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glm::mat4 viewMatrix, modelMatrix;
   
   modelMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(5.0f));
   viewMatrix = glm::lookAt(glm::vec3(3.0, 3.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1.0, 0));
   
   for (auto& shaderPair: shaders_.getMap()) {
      shaderPair.second.use();
      
      shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::MODEL),
         modelMatrix);
      shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::VIEW),
         viewMatrix);
      shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::PROJECTION),
         glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 100.f));
      
      if(shaderPair.first == ShaderType::TEXTURE) {
         enableTexture(0);
         shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::TEXTURE), 0);
      }
      
      if(shaderPair.first == ShaderType::SUN) {
          Material mat;
          mat.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
          mat.diffuse = glm::vec3(0.7f, 0.5f, 0.7f);
          mat.specular = glm::vec3(0.1f, 0.2f, 0.1f);
          mat.shine = 100.0f;
          
         shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::M_AMB),
            mat.ambient);
         shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::M_DIF),
            mat.diffuse);
         shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::M_SPEC),
            mat.specular);
         shaderPair.second.sendUniform(shaders_.getUniforms(Uniform::M_SHINE),
            mat.shine);
      }
      
      shaderPair.second.drawMesh(ground_.mesh_);

      if(shaderPair.first == ShaderType::TEXTURE)
         disableTexture();
   }
}

void Game::mainLoop() {
   loadTexture(texture_path(Texture::WATER));
   bunny = Mesh::fromAssimpMesh(shaders_,loadMesh("../models/cube.obj"));
   
   Input input;
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
