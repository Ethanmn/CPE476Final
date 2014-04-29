#include "sound_engine.h"

#include <iostream>
#include <irrKlang.h>

SoundEngine::SoundEngine() {
   engine_ = irrklang::createIrrKlangDevice();
   if (!engine_) {
      std::cerr << "Could not load irrklang engine" << std::endl;
   }

   rustle_sound_source_ = engine_->addSoundSourceFromFile("../sounds/rustle1.ogg", irrklang::ESM_NO_STREAMING, true);
}

void SoundEngine::play3D(SoundType sound, const glm::vec3& source_position) {
   if (!engine_->isCurrentlyPlaying(rustle_sound_source_)) {
      engine_->play3D(rustle_sound_source_, irrklang::vec3df(0, 0, 0), false);
   }
}
