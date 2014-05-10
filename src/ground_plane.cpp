#include "ground_plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"

const int GroundPlane::GROUND_SCALE = 500;

const std::vector<unsigned short> ground_indices{
   0, 2, 1, 3, 1, 2
};

GroundPlane::GroundPlane(const Mesh& mesh) :
   texture_(texture_path(Textures::GRASS)),
   height_map_(texture_path(Textures::HEIGHT_MAP)),
   mesh_(mesh),
   // Load it twice because textures confuse me.
   height_map_image_(texture_path(Textures::HEIGHT_MAP)) {

   const glm::mat4 translate(glm::translate(glm::mat4(), glm::vec3(0, -7, 0)));
   const glm::mat4 scale(glm::scale(glm::mat4(1.0), glm::vec3(GROUND_SCALE, 1, GROUND_SCALE)));
   transform_ = translate * scale;

}

void GroundPlane::draw(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {

   setupModelView(shader, uniform_locations, transform_, viewMatrix, true);
   setupTextureShader(shader, uniform_locations, texture_);
   setupHeightMap(shader, uniform_locations, height_map_);
   shader.drawMesh(mesh_);
   height_map_.disable();
   shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniform_locations, 0);
   texture_.disable();
}

float GroundPlane::heightAt(const glm::vec3& position) const {
   glm::vec4 pos(position, 1.0f);
   // 1.translate position from world into texture space.
      // a. translate position from world into mesh space.
   pos = glm::inverse(transform_) * pos;
      // b. translate position from mesh into texture space.
      // TODO(chebert): this is a total hack. we assume that the mesh is 2x2x0
      // centered at the origin, and not rotated. I'm sorry but the deadline is
      // monday.
   pos = glm::translate(glm::mat4(), glm::vec3(0.5f, 0, 0.5f)) *
         glm::scale(glm::mat4(), glm::vec3(0.5f)) * pos;

   // if within  [0-1], [0-1]
   if (0.0f <= pos.x && pos.x <= 1.0f &&
       0.0f <= pos.z && pos.z <= 1.0f) {
      // Translate from texture space to image space.
      // Invert the z, because image measures from the top-left (not
      // bottom-left)
      pos.z = 1.0f - pos.z;
      auto pixel_packet = height_map_image_.getConstPixels(
            (int)round(pos.x * height_map_image_.columns()),
            (int)round(pos.z * height_map_image_.rows()), 1, 1);
      //std::clog <<
         //(int)round(pos.x * height_map_image_.columns()) <<
         //", " << 
         //(int)round(pos.z * height_map_image_.rows()) << ": " <<
         //pixel_packet->red << std::endl;
      // find r/g/ or b value at given coordinate.
      // normalize the coordinate about [-.5,.5]
      // multiply by 15.0f
      return (pixel_packet->red / 65535.0f - 0.5f) * 15.0f;
   }
   // return a height of 0 if we are out of bounds (for testing).
   return 0.0f;
}
