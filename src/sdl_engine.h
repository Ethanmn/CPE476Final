#ifndef SDL_ENGINE_H_
#define SDL_ENGINE_H_

#include <iostream>
#include <SDL.h>
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
      window_ = SDL_CreateWindow("Deer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            kScreenWidth, kScreenHeight, 
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED);
      context_ = SDL_GL_CreateContext(window_);
      SDL_GL_MakeCurrent(window_, context_);
      glewInit();
      std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
      std::cout << "Shader version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
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
