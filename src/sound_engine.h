#ifndef SOUND_ENGINE_H_
#define SOUND_ENGINE_H_

#include <glm/glm.hpp>

namespace irrklang {
   class ISoundEngine;
   class ISoundSource;
}

struct SoundEngine {
   enum class SoundType {
      RUSTLE
   };
   SoundEngine();

   void play3D(SoundType sound, const glm::vec3& source_position);

  private:
   irrklang::ISoundEngine* engine_;
   irrklang::ISoundSource* rustle_sound_source_;
};

#endif // SOUND_ENGINE_H_
