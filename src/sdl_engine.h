#ifndef SDL_ENGINE_H_
#define SDL_ENGINE_H_

#include <iostream>
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#ifdef _WIN32
    #undef main
#endif

#include "globals.h"

struct SDLEngine {
   SDLEngine() {
      SDL_Init(SDL_INIT_VIDEO);
      //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
      window_ = SDL_CreateWindow("Deer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kScreenWidth, kScreenHeight, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED);
      context_ = SDL_GL_CreateContext(window_);
      SDL_GL_MakeCurrent(window_, context_);
      if (GLEW_OK != glewInit()) {
         std::cerr << "Glew did not init right" << std::endl;
         exit(EXIT_FAILURE);
      }
      std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
      std::cout << "Shader version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
      if (glewGetExtension("GL_EXT_framebuffer_object")) {
         std::cout << "framebuffer is available" << std::endl;
      }

      glClearDepth(1.0f);
      glDepthFunc(GL_LESS);
      glEnable(GL_DEPTH_TEST);// Enable Depth Testing
      glDisable(GL_LIGHTING);
      glShadeModel(GL_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glEnable(GL_CULL_FACE);

      glPolygonMode(GL_FRONT, GL_LINE);
      glLineWidth(1.0);

      //glEnable(GL_BLEND);
      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

   ~SDLEngine() {
      SDL_GL_DeleteContext(context_);
      SDL_DestroyWindow(window_);
      SDL_Quit();
   }

   void swapWindow() { SDL_GL_SwapWindow(window_); }

  private:
   SDL_Window* window_;
   SDL_GLContext context_;
};

#endif // SDL_ENGINE_H_
