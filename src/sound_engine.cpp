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

   sound_effect_sources_[SoundEffect::THUNDER_STRIKE] =
      engine_->addSoundSourceFromFile(
            "../sounds/thunder_strike.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::EAT_FLOWER] =
   engine_->addSoundSourceFromFile(
            "../sounds/eating.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::TREE_HIT] =
   engine_->addSoundSourceFromFile(
            "../sounds/tree_hit.ogg",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::EAT_FLOWER]->setDefaultVolume(0.5f);

      sound_effect_sources_[SoundEffect::WATER] =
      engine_->addSoundSourceFromFile(
            "../sounds/water.wav",
            irrklang::ESM_NO_STREAMING,
            should_preload);
   sound_effect_sources_[SoundEffect::WATER]->setDefaultVolume(0.8f);
}

void SoundEngine::set_listener_position(const glm::vec3& position, const glm::vec3& orientation) {
   engine_->setListenerPosition(
         irrklang::vec3df(position.x, position.y, position.z),
         irrklang::vec3df(orientation.x, orientation.y, orientation.z));
}

void SoundEngine::playSoundEffect(SoundEffect sound, bool should_loop, const glm::vec3& /*source_position*/) {
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

inline std::string songPath(SoundEngine::Song song) {
   switch (song) {
      case SoundEngine::Song::DAY_SONG:
         return "../music/rhapsody_in_blue-Gershwin.ogg";
      case SoundEngine::Song::NIGHT_SONG:
         return "../music/moonlight_sonata_mvmt_1.ogg";
      case SoundEngine::Song::STORM_SONG:
         return "../music/moonlight_sonata_mvmt_3.ogg";
   }
}

irrklang::ISound* SoundEngine::loadSong(Song song) {
   auto* sound = engine_->play2D(songPath(song).c_str(), false, true, true);
   sound->setVolume(1.5f);
   return sound;
}
