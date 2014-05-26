#include <math.h>
#include "dist.h"

float dist(glm::vec3 p0, glm::vec3 p1) {
	float px, py, pz;
	px = (p0.x - p1.x) * (p0.x - p1.x);
	py = (p0.y - p1.y) * (p0.y - p1.y);
	pz = (p0.z - p1.z) * (p0.z - p1.z);

	return sqrt(px + py + pz);
}