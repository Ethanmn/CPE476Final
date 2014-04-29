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
      RUSTLE
   };
   SoundEngine();

   void playSoundEffect(SoundEffect sound, const glm::vec3& source_position);

  private:
   irrklang::ISoundEngine* engine_;
   std::map<SoundEffect, irrklang::ISoundSource*> sound_effect_sources_;
};

#endif // SOUND_ENGINE_H_
