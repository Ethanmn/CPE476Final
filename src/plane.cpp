#include "plane.h"

void Plane::set3Points(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
   v = p1 - p0;
   u = p2 - p0;
   n = glm::normalize(glm::cross(v, u));
   A = n.x;
   B = n.y;
   C = n.z;
   D = glm::dot(-n, p0);
}

float Plane::distance(glm::vec3 p) {
   return glm::dot(n, p) + D;
}