/* 
   Camera.h
   Katie Keim
   Deer - CPE 476
*/

#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Camera {
   Camera();
   Camera(glm::vec3 position, glm::vec3 lookAt);

   void setPosition(glm::vec3 endPosition);
   void moveCameraToPoint(glm::vec3& endPosition, float speed);
   void moveCameraInDirection(glm::vec3& direction, float speed);
   void rotateWithDrag(glm::vec3& startPoint, glm::vec3& endPoint, int width, int height);
   glm::vec3 getPosition();
   glm::vec3 getCamForwardVec();
   glm::vec3 getCamLeftVec();
   glm::mat4 getViewMatrix();
   
   private:
      glm::vec3 position;
      glm::vec3 lookAt;
      glm::vec3 up;
      float phi;
      float theta;

      void updateLookAt();
};

#endif // CAMERA_H_

