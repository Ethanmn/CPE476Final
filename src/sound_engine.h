#ifndef SOUND_ENGINE_H_
#define SOUND_ENGINE_H_

#include <glm/glm.hpp>
#include <map>

namespace irrklang {
   class ISoundEngine;
   class ISoundSource;
}

struct SoundEngine {
   enum class SoundEffect {
      RUSTLE,
      GRASS_WALK0,
      GRASS_WALK1,
      GRASS_WALK2,
      GRASS_WALK3,
      GRASS_WALK4,
      GRASS_LAND,

      CARDINAL_BIRD,
      CANARY0,
      CANARY1,
   };
   SoundEngine();

   void set_listener_position(const glm::vec3& listener_position, const glm::vec3& orientation);
   void playSoundEffect(SoundEffect sound, bool should_loop, const glm::vec3& source_position);
   void playRandomWalkSound();

  private:
   irrklang::ISoundEngine* engine_;
   std::map<SoundEffect, irrklang::ISoundSource*> sound_effect_sources_;
};

#endif // SOUND_ENGINE_H_
