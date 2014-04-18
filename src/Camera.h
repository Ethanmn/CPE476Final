#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define PI 3.14159265359
#define MAX_ROTATE_VERT 80
#define PI_IN_DEGREES 180

struct Camera {
   Camera();
   Camera(glm::vec3 position, glm::vec3 lookAt);

   void setPosition(glm::vec3 endPosition);
   void moveCameraToPoint(glm::vec3 endPosition, float speed);
   void moveCameraInDirection(glm::vec3 direction, float speed);
   void rotateWithDrag(glm::vec3 startPoint, glm::vec3 endPoint, int width, int height);
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
};

#endif // CAMERA_H_

