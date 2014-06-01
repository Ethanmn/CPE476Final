#include "plane.h"

Plane::Plane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
   const auto v = p1 - p0;
   const auto u = p2 - p0;
   normal_ = glm::normalize(glm::cross(v, u));
   D = glm::dot(-normal_, p0);
}

float Plane::distance(glm::vec3 p) {
   return glm::dot(normal_, p) + D;
}
