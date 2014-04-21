#ifndef GAME_H_
#define GAME_H_

#include <glm/glm.hpp>
#include <vector>

#include "deer.h"
#include "input.h"
#include "sdl_engine.h"
#include "units.h"
#include "graphics/shaders.h"

struct Game {
   Game();

   void mainLoop();

  private:
   SDLEngine engine_;
   Input input_;
   Shaders shaders_;
   AttributeLocationMap attribute_location_map_;
   UniformLocationMap uniform_location_map_;
   Deer deer_;

   void step(units::MS dt);
   void draw();
};

#endif // GAME_H_
