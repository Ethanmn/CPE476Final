#ifndef PLANE_H_
#define PLANE_H_

#include <glm/glm.hpp>

struct Plane {

private:
glm::vec3 p0, p1, p2, u, v, n;
float A, B, C, D;

public:
	void set3Points(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	float distance(glm::vec3 p);
};

#endif //PLANE_H_