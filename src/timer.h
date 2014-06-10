#ifndef TIMER_H_
#define TIMER_H_

#include <SDL.h>
#include <iostream>
#include "units.h"

struct Timer {
   void start() {
      start_time = SDL_GetTicks();
   }

   void end() {
      average_time = (average_time * num_times + SDL_GetTicks() - start_time) / (num_times + 1);
      ++num_times;
      if (num_times == 10) {
         std::clog << 1000 / average_time << std::endl;
         average_time = num_times = 0;
      }
   }

  private:
   units::MS average_time = 0, num_times = 0, start_time = 0;
};

#endif // TIMER_H_
