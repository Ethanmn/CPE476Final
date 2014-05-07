#ifndef SDL_ENGINE_H_
#define SDL_ENGINE_H_

#include <SDL.h>
#include <GL/glew.h>
#ifdef _WIN32
    #undef main
#endif

#include "globals.h"

struct SDLEngine {
   SDLEngine() {
      SDL_Init(SDL_INIT_VIDEO);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
      window_ = SDL_CreateWindow("Deer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kScreenWidth, kScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED);
      context_ = SDL_GL_CreateContext(window_);
      SDL_GL_MakeCurrent(window_, context_);
      //SDL_SetRelativeMouseMode((SDL_bool)true);
      glewInit();
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
