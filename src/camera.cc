#include "camera.h"
#include "config.h"

#include <cmath>

Camera::Camera(unsigned int w, unsigned int h, real fov)
    : width(w), height(h), mul(tan(fov * 0.5)) {}

void Camera::moveTo(Vector pos) { transform.d = pos; }

void Camera::lookAt(Vector pos) {
  Vector rel = pos - transform.d;
  transform.a = rel.normalized();
  Vector tmp = cross({0, 0, 1}, transform.a);
  if (dot(tmp, tmp) < EPSILON) tmp = {0, 1, 0};
  transform.b = tmp;
  transform.c = cross(transform.a, transform.b);
}

unsigned int Camera::getWidth() const { return width; }

unsigned int Camera::getHeight() const { return height; }

Ray Camera::getRay(real x, real y, unsigned int mask) {
  real big = width > height ? width : height;
  Vector start = transform.d;
  Vector base_ray = {1,
                     mul * (width - 2 * x) / big,
                     mul * (height - 2 * y) / big};
  Vector direction = transform(base_ray);
  return Ray(start, direction, mask);
}
