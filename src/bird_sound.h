#ifndef BIRD_SOUND_H_
#define BIRD_SOUND_H_

#include <glm/glm.hpp>

#include "units.h"

struct SoundEngine;

struct BirdSound {
   BirdSound();

   void step(units::MS dt, SoundEngine& sound_engine);

  private:
   units::MS countdown_timer_;
   units::MS next_time_;

   glm::vec3 position_;
};

#endif // BIRD_SOUND_H_
