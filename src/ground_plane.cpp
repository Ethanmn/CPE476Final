#include "ground_plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"

const int GroundPlane::GROUND_SCALE = 500;
const int PLANE_SIZE = 50;

const float HEIGHT_MAP_SCALE = 3.0f;

const std::vector<unsigned short> ground_indices{
   0, 2, 1, 3, 1, 2
};

GroundPlane::GroundPlane(const Mesh& mesh) :
   texture_(texture_path(Textures::GRASS)),
   height_map_(texture_path(Textures::HEIGHT_MAP)),
   mesh_(mesh),
   // TODO(chebert): Loaded it twice because textures confuse me.
   height_map_image_(texture_path(Textures::HEIGHT_MAP)) {

   const glm::mat4 scale(glm::scale(glm::mat4(1.0), glm::vec3(PLANE_SIZE, 1, PLANE_SIZE)));
   //COLUMN MAJOR
   for (int x = -GROUND_SCALE/2; x < GROUND_SCALE/2; x += PLANE_SIZE) {
      for (int y = -GROUND_SCALE/2; y < GROUND_SCALE/2; y += PLANE_SIZE) {
         transforms_.push_back(
               glm::translate(glm::mat4(), glm::vec3(x, 0, y)) * scale);
      }
   }
}

void GroundPlane::draw(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupTextureShader(shader, uniform_locations, texture_);
   setupHeightMap(shader, uniform_locations, height_map_);

   for (auto& t : transforms_) {
      setupModelView(shader, uniform_locations, t, viewMatrix, true);
      shader.drawMesh(mesh_);
   }

   height_map_.disable();
   shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniform_locations, 0);
   texture_.disable();
}

void GroundPlane::shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, glm::vec3 deerLoc, bool betterShadow) {
   setupHeightMap(shader, uniform_locations, height_map_);

   for (auto& t : transforms_) {
      if(betterShadow)
         setupBetterShadowShader(shader, uniform_locations, sunDir, deerLoc, t);
      else
         setupShadowShader(shader, uniform_locations, sunDir, deerLoc, t);
      
      shader.drawMesh(mesh_);
   }

   height_map_.disable();
   shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniform_locations, 0);
}

float GroundPlane::heightAt(const glm::vec3& position) const {
   glm::vec4 pos(position, 1.0f);
   // 1.translate position from world into texture space.
      // a. determine which ground plane to test.
   const int row = (pos.z + GROUND_SCALE / 2) / PLANE_SIZE;
   const int col = (pos.x + GROUND_SCALE / 2) / PLANE_SIZE;
   const int index = (col * GROUND_SCALE / PLANE_SIZE) + row;
   if (row < 0 || col < 0 || (size_t)index >= transforms_.size()) {
      std::clog << "Warning: out of bounds" << std::endl;
      return 0.0f;
   }
      // b. translate position from world into mesh space.
   pos = glm::inverse(transforms_.at(index)) * pos;
      // c. translate position from mesh into texture space.
      // TODO(chebert): this is a total hack. we assume that the mesh is 2x2x0
      // centered at the origin, and rotated (C?)CW 90 degrees. deadline is monday.
   pos = glm::translate(glm::mat4(), glm::vec3(0.5f, 0, 0.5f)) *
         glm::rotate(glm::mat4(), -90.0f, glm::vec3(0, 1, 0)) *
         glm::scale(glm::mat4(), glm::vec3(0.5f)) * pos;

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
      return (pixel_packet->red / 65535.0f - 0.5f) * HEIGHT_MAP_SCALE;
   }
   // return a height of 0 if we are out of bounds (for testing).
   return 0.0f;
}
