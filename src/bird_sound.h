#ifndef BIRD_SOUND_H_
#define BIRD_SOUND_H_

#include <glm/glm.hpp>

#include "units.h"
#include "sound_engine.h"

struct BirdSound {
   BirdSound(SoundEngine::SoundEffect sound, units::MS period);

   void step(units::MS dt, SoundEngine& sound_engine);

  private:
   units::MS calculate_next_time() const;

   const units::MS period_;
   units::MS countdown_timer_;
   units::MS next_time_;
   SoundEngine::SoundEffect sound_;

   glm::vec3 position_;
};

#endif // BIRD_SOUND_H_
