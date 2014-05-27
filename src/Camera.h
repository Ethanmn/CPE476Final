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

struct Camera {
   Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(0, 0, 1));

   void setPosition(const glm::vec3& endPosition);
   void setLookAt(const glm::vec3& lookAtPoint);
   glm::vec3 getPosition() const;
   glm::vec3 getLookAt() const;
   glm::vec3 getCamForwardVec() const;
   glm::vec3 getCamLeftVec() const;
   glm::mat4 getViewMatrix() const;

   void step(float dT);
   
   private:
      glm::vec3 position;
      glm::vec3 lookAt;
      glm::vec3 up;

      glm::vec3 target;

      float springStrength;
      float dampConst;
};

#endif // CAMERA_H_

