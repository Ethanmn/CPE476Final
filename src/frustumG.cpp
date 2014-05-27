#include "frustumG.h"
#include <iostream>

#define ANG2RAD 3.14159265358979323846/180.0

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

	// store the information
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = (float)tan(ANG2RAD * angle * 0.5);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD * tang;
	fw = fh * ratio;
}

void FrustumG::setCamDef(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u) {

	glm::vec3 dir, nc, fc, X, Y, Z;

	// compute the Z axis of camera
	// this axis points in the opposite direction from
	// the looking direction
	Z = glm::normalize(p - l);

	// X axis of camera with given "up" vector and Z axis
	X = glm::normalize(u * Z);

	// the real "up" vector is the cross product of Z and X
	Y = glm::normalize(glm::cross(Z, X));

	// compute the centers of the near and far planes
	nc = p - Z * nearD;
	fc = p - Z * farD;

	// compute the 4 corners of the frustum on the near plane
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	// compute the 4 corners of the frustum on the far plane
	ftl = (fc + Y * fh - X * fw);
	ftr = (fc + Y * fh + X * fw);
	fbl = (fc - Y * fh - X * fw);
	fbr = (fc - Y * fh + X * fw);

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr,ntl,ftl);
	pl[BOTTOM].set3Points(nbl,nbr,fbr);
	pl[LEFT].set3Points(ntl,nbl,fbl);
	pl[RIGHT].set3Points(nbr,ntr,fbr);
	pl[NEARP].set3Points(ntl,ntr,nbr);
	pl[FARP].set3Points(ftr,ftl,fbl);
}

int FrustumG::pointInFrustum(glm::vec3 &p) {

	int result = INSIDE;

	for (int i = 0; i < 6; i++) {
		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return result;
}

int FrustumG::sphereInFrustum(glm::vec3 &p, float radius) {

	float distance;
	int result = INSIDE;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < (-radius) - 15)
			return OUTSIDE;
		else if (distance < radius)
			result = INTERSECT;
	}
	return result;
}
