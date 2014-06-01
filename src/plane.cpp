#include "plane.h"

Plane::Plane(const glm::vec3& normal, float D) :
   normal_(normal),
   D(D)
{
   float mag = glm::length(normal);
   normal_ /= mag;
   this->D /= mag;
}

float Plane::distance(const glm::vec3& p) {
   return glm::dot(normal_, p) + D;
}
