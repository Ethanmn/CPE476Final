#include "bird_sound.h"

#include <stdlib.h>

#include "sound_engine.h"

BirdSound::BirdSound(SoundEngine::SoundEffect sound, units::MS period) :
   period_(period),
   countdown_timer_(0),
   next_time_(calculate_next_time()),
   sound_(sound)
{}

void BirdSound::step(units::MS dt, SoundEngine& sound_engine) {
   countdown_timer_ += dt;
   if (countdown_timer_ > next_time_) {
      countdown_timer_ = 0;
      next_time_ = calculate_next_time();
      sound_engine.playSoundEffect(sound_, false, position_);
   }
}

units::MS BirdSound::calculate_next_time() const {
   return period_ + rand() % 2000;
}
