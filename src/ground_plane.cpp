#include "ground_plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/texture.h"
#include "globals.h"

const int GroundPlane::GROUND_SCALE = 1000;

const std::vector<unsigned short> ground_indices{
   0, 2, 1, 3, 1, 2
};

GroundPlane::GroundPlane(const Mesh& mesh) :
   drawable_({
         DrawTemplate({
            ShaderType::TEXTURE,
            mesh,
            Material(),
            Texture(TextureType::GRASS, DIFFUSE_TEXTURE), 
            Texture(TextureType::HEIGHT_MAP, HEIGHT_MAP_TEXTURE),
            EffectSet({EffectType::CASTS_SHADOW})
            }),
         std::vector<glm::mat4>({glm::scale(glm::mat4(1.0), glm::vec3(GROUND_SCALE, 1, GROUND_SCALE))}),
         }),
   // TODO(chebert): Loaded it twice because textures confuse me.
   height_map_image_(texture_path(TextureType::HEIGHT_MAP)) { }

float GroundPlane::heightAt(const glm::vec3& position) const {
   glm::vec4 pos(position, 1.0f);
   // 1.translate position from world into texture space.
   // a. determine which ground plane to test. OBSOLETE
   // b. translate position from world into mesh space.
   pos = glm::inverse(drawable_.model_transforms.front()) * pos;
   // c. translate position from mesh into texture space.
   // TODO(chebert): this is a total hack. we assume that the mesh is
   // centered at the origin, and rotated (C?)CW 90 degrees. deadline is monday.
   pos = glm::translate(glm::mat4(), glm::vec3(0.5f, 0, 0.5f)) *
      glm::rotate(glm::mat4(), -90.0f, glm::vec3(0, 1, 0)) * pos;

   // if within  [0-1], [0-1]
   if (0.0f <= pos.x && pos.x <= 1.0f &&
         0.0f <= pos.z && pos.z <= 1.0f) {
      // Translate from texture space to image space.
      auto pixel_packet = height_map_image_.getConstPixels(
            (int)round(pos.x * height_map_image_.columns()),
            (int)round(pos.z * height_map_image_.rows()), 1, 1);
      // find r/g/ or b value at given coordinate.
      // normalize the coordinate about [-.5,.5]
      // multiply by some constant
      // TODO(chebert): This should match the texture shader. Deadline monday,
      // so I am postponing good coding stuffz.
      return (pixel_packet->red / 65535.0f - 0.5f) * kHeightMapScale;
   }
   // return a height of 0 if we are out of bounds (for testing).
   return 0.0f;
}
