/* 
   Camera.h
   Katie Keim
   Deer - CPE 476
*/

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "units.h"
#include "globals.h"

struct Deer;
struct Camera {
   enum class Position {
      BEHIND,
      FRONT_RIGHT,
      LEFT
   };

   Camera(glm::vec3 position, glm::vec3 lookAt);
   glm::mat4 getViewMatrix() const;

   void step(float dT, const glm::vec3& target_pos, const glm::vec3& target_facing, Position relative_position);

   glm::vec3 get_position() const { return position; }

  private:
   glm::vec3 position;
   glm::vec3 lookAt;
   glm::vec3 camera_target;
};

#endif // CAMERA_H_

