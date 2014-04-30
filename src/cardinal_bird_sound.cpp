#include "cardinal_bird_sound.h"

#include <stdlib.h>

#include "sound_engine.h"

namespace {
   units::MS calculate_next_time() {
      return 10000 + rand() % 2000;
   }
}

CardinalBirdSound::CardinalBirdSound() :
   countdown_timer_(0),
   next_time_(calculate_next_time())
{}

void CardinalBirdSound::step(units::MS dt, SoundEngine& sound_engine) {
   countdown_timer_ += dt;
   if (countdown_timer_ > next_time_) {
      countdown_timer_ = 0;
      next_time_ = calculate_next_time();
      sound_engine.playSoundEffect(SoundEngine::SoundEffect::CARDINAL_BIRD, false, position_);
   }
}
