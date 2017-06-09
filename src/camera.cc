#include "camera.h"
#include "config.h"

#include <algorithm>
#include <cmath>

Camera::Camera(unsigned int width, unsigned int height, real field_of_view)
    : width_(width), height_(height),
      scale_factor_(std::tan(field_of_view * 0.5) / std::max(width, height)) {}

void Camera::MoveTo(Vector position) { transform_.d = position; }

void Camera::LookAt(Vector position) {
  Vector offset = position - transform_.d;
  transform_.a = offset.normalized();
  Vector left = cross({0, 0, 1}, transform_.a);
  if (dot(left, left) < EPSILON) left = {0, 1, 0};
  transform_.b = left;
  transform_.c = cross(transform_.a, transform_.b);
}

Ray Camera::GetRay(real x, real y, unsigned int mask) {
  Vector start = transform_.d;
  Vector base_ray =
      {1, scale_factor_ * (width_ - 2 * x), scale_factor_ * (height_ - 2 * y)};
  Vector direction = transform_(base_ray);
  return Ray(start, direction, mask);
}
