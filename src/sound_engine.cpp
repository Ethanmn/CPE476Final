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
   sound_effect_sources_[SoundEffect::RUSTLE]->setDefaultVolume(0.6f);

   sound_effect_sources_[SoundEffect::GRASS_LAND] =
      engine_->addSoundSourceFromFile(
            "../sounds/grassLand.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);

   sound_effect_sources_[SoundEffect::GRASS_WALK0] =
      engine_->addSoundSourceFromFile(
            "../sounds/grassStep1.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::GRASS_WALK1] =
      engine_->addSoundSourceFromFile(
            "../sounds/grassStep2.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::GRASS_WALK2] =
      engine_->addSoundSourceFromFile(
            "../sounds/grassStep3.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::GRASS_WALK3] =
      engine_->addSoundSourceFromFile(
            "../sounds/grassStep4.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::GRASS_WALK4] =
      engine_->addSoundSourceFromFile(
            "../sounds/grassStep5.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);

   const auto kGrassVolume = 0.3f;
   sound_effect_sources_[SoundEffect::GRASS_LAND]->setDefaultVolume(kGrassVolume);
   sound_effect_sources_[SoundEffect::GRASS_WALK0]->setDefaultVolume(kGrassVolume);
   sound_effect_sources_[SoundEffect::GRASS_WALK1]->setDefaultVolume(kGrassVolume);
   sound_effect_sources_[SoundEffect::GRASS_WALK2]->setDefaultVolume(kGrassVolume);
   sound_effect_sources_[SoundEffect::GRASS_WALK3]->setDefaultVolume(kGrassVolume);
   sound_effect_sources_[SoundEffect::GRASS_WALK4]->setDefaultVolume(kGrassVolume);

   sound_effect_sources_[SoundEffect::CARDINAL_BIRD] =
      engine_->addSoundSourceFromFile(
            "../sounds/cardinal_bird.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::CARDINAL_BIRD]->setDefaultVolume(0.1f);

   sound_effect_sources_[SoundEffect::CANARY0] =
      engine_->addSoundSourceFromFile(
            "../sounds/canary0.wav",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::CANARY0]->setDefaultVolume(0.3f);
   sound_effect_sources_[SoundEffect::CANARY1] =
      engine_->addSoundSourceFromFile(
            "../sounds/canary1.wav",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::CANARY1]->setDefaultVolume(0.2f);

   sound_effect_sources_[SoundEffect::WOODPECKER0] =
      engine_->addSoundSourceFromFile(
            "../sounds/woodpecker1.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::WOODPECKER1] =
      engine_->addSoundSourceFromFile(
            "../sounds/woodpecker2.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::WOODPECKER2] =
      engine_->addSoundSourceFromFile(
            "../sounds/woodpecker3.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::WOODPECKER3] =
      engine_->addSoundSourceFromFile(
            "../sounds/woodpecker4.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::WOODPECKER0]->setDefaultVolume(0.2f);
}

void SoundEngine::set_listener_position(const glm::vec3& position, const glm::vec3& orientation) {
   engine_->setListenerPosition(
         irrklang::vec3df(position.x, position.y, position.z),
         irrklang::vec3df(orientation.x, orientation.y, orientation.z));
}

void SoundEngine::playSoundEffect(SoundEffect sound, bool should_loop, const glm::vec3& source_position) {
   if (!engine_->isCurrentlyPlaying(sound_effect_sources_[sound])) {
      /* TODO(chebert): 3d Sound sound legitimately bad. I think i has to do
       * with the listener direction.
      engine_->play3D(
            sound_effect_sources_[sound],
            irrklang::vec3df(source_position.x, source_position.y, source_position.z),
            should_loop);
            */
      engine_->play2D(
            sound_effect_sources_[sound],
            should_loop);
   }
}

void SoundEngine::playRandomWalkSound() {
   const auto val = rand() % 3;
   const auto walk = static_cast<SoundEffect>(val + static_cast<int>(SoundEffect::GRASS_WALK0));
   engine_->play2D(sound_effect_sources_[walk], false);
}