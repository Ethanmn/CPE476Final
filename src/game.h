#ifndef GAME_H_
#define GAME_H_

#include <glm/glm.hpp>
#include <vector>

#include "input.h"
#include "sdl_engine.h"
#include "units.h"

struct Game {
   Game();

   void mainLoop();

  private:
   SDLEngine engine_;
   Input input_;

   void step(units::MS dt);
   void draw();
};

#endif // GAME_H_
