/* 
   Camera.h
   Katie Keim
   Deer - CPE 476
*/

#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "units.h"

struct Camera {
   Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(0, 0, 1));

   void setPosition(const glm::vec3& endPosition);
   void setLookAt(const glm::vec3& lookAtPoint);
   void moveCameraToPoint(const glm::vec3& endPosition, float speed, units::MS timeDif);
   void moveCameraInDirection(const glm::vec3& direction, float speed, units::MS timeDif);
   void rotateLookAtWithDrag(const glm::vec2& startPoint, const glm::vec2& endPoint, int width, int height);
   glm::vec3 getPosition() const;
   glm::vec3 getCamForwardVec() const;
   glm::vec3 getCamLeftVec() const;
   glm::mat4 getViewMatrix() const;
   void rotatePositionWithDrag(const glm::vec2& startPoint, const glm::vec2& endPoint, int width, int height);
   
   private:
      glm::vec3 position;
      glm::vec3 lookAt;
      glm::vec3 up;
      float phi;
      float theta;

      void updateLookAt();
      void updatePosition();
};

#endif // CAMERA_H_

