#include "camera.h"
#include "config.h"

#include <algorithm>
#include <cmath>

Camera::Camera(int width, int height, real field_of_view)
    : width_(width), height_(height),
      scale_factor_(std::tan(field_of_view * 0.5) / std::max(width, height)) {}

void Camera::MoveTo(Vector position) { transform_.d_ = position; }

void Camera::LookAt(Vector position) {
  Vector offset = position - transform_.d_;
  transform_.a_ = offset.normalized();
  Vector left = cross({0, 0, 1}, transform_.a_);
  if (dot(left, left) < EPSILON) left = {0, 1, 0};
  transform_.b_ = left;
  transform_.c_ = cross(transform_.a_, transform_.b_);
}

Ray Camera::GetRay(real x, real y) const {
  Vector start = transform_.d_;
  Vector base_ray =
      {1, scale_factor_ * (width_ - 2 * x), scale_factor_ * (height_ - 2 * y)};
  Vector direction = transform_(base_ray).normalized();
  return Ray{start, direction};
}
