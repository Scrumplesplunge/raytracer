#include "sky.h"

#include <cmath>

Sky::Sky(const char* filename) : texture_(Image::Load(filename)) {}

Vector Sky::OutgoingLight(const Shape* /* scene */, const TraceRes& /* hit */,
                          Vector direction, real /* significance */) const {
  Vector rayDir = -direction;

  real v = std::acos(rayDir.z) / PI;
  Vector tmp = Vector{rayDir.x, rayDir.y, 0}.normalized();
  real u = tmp.x > 0 ? std::acos(tmp.y) / TWOPI : 1 - std::acos(tmp.y) / TWOPI;

  Pixel pixel = texture_(
      (static_cast<int>(u * (texture_.width() - 1))) % texture_.width(),
      (static_cast<int>(v * (texture_.height() - 1))) % texture_.height());

  Vector color = {static_cast<real>(pixel.red),
                  static_cast<real>(pixel.green),
                  static_cast<real>(pixel.blue)};
  return color / 255;
}
