#include "sound_engine.h"

#include <iostream>
#include <irrKlang.h>

const float kRollOff = 5.0f; // 1.0 corresponds to real world. 10.0 is the max

SoundEngine::SoundEngine() {
   engine_ = irrklang::createIrrKlangDevice();
   if (!engine_) {
      std::cerr << "Could not load irrklang engine" << std::endl;
   }
   engine_->setRolloffFactor(kRollOff);

   const auto should_preload = true;
   sound_effect_sources_[SoundEffect::RUSTLE] =
      engine_->addSoundSourceFromFile(
            "../sounds/rustle1.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
}

void SoundEngine::set_listener_position(const glm::vec3& position, const glm::vec3& orientation) {
   engine_->setListenerPosition(
         irrklang::vec3df(position.x, position.y, position.z),
         irrklang::vec3df(orientation.x, orientation.y, orientation.z));
}

void SoundEngine::playSoundEffect(SoundEffect sound, const glm::vec3& source_position) {
   const auto should_loop = false;
   if (!engine_->isCurrentlyPlaying(sound_effect_sources_[sound])) {
      engine_->play3D(
            sound_effect_sources_[SoundEffect::RUSTLE],
            irrklang::vec3df(source_position.x, source_position.y, source_position.z),
            should_loop);
   }
}
