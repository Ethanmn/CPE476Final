#ifndef PLANE_H_
#define PLANE_H_

#include <glm/glm.hpp>

struct Plane {
   Plane(const glm::vec3& normal, float D);
   float distance(const glm::vec3& p);

private:
   glm::vec3 normal_; // A, B, C
   float D;
};

#endif //PLANE_H_
