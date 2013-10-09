#include "Camera.h"
#include "Config.h"
#include <cmath>

Camera::Camera(unsigned int w, unsigned int h, real fov) : width(w), height(h), mul(tan(fov * 0.5)) {}

void Camera::moveTo(const Vector& pos) {
	transform.d = pos;
}

void Camera::lookAt(const Vector& pos) {
	Vector rel = pos - transform.d;
	transform.a = rel.normalized();
	Vector tmp = cross(transform.a, Vector(0, 0, 1));
	if (tmp.squareLength() < EPSILON) tmp = Vector(0, 1, 0);
	transform.b = tmp;
	transform.c = cross(transform.b, transform.a);
}

Ray Camera::getRay(real x, real y, unsigned int mask) {
	real big = width > height ? width : height;
	Vector direction(transform(Vector(1, mul * (2 * x - width) / big, mul * (height - 2 * y) / big)));
	Vector start(transform.d);
	return Ray(start, direction, mask);
}

