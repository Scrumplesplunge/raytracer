#include "sky.h"

#include <cmath>

Sky::Sky(const char* filename) : Image(filename) {}

Vector Sky::outgoingLight(const Shape* /* scene */, const TraceRes& /* hit */,
                          Vector direction, real /* significance */) const {
  Vector rayDir = -direction;

  real v = std::acos(rayDir.z) / PI;
  Vector tmp = Vector{rayDir.x, rayDir.y, 0}.normalized();
  real u = tmp.x > 0 ? std::acos(tmp.y) / TWOPI : 1 - std::acos(tmp.y) / TWOPI;

  unsigned char *pix = operator()(
      ((unsigned int)(u * (getWidth() - 1))) % getWidth(),
      ((unsigned int)(v * (getHeight() - 1))) % getHeight());
  real red = pix[2], green = pix[1], blue = pix[0];
  return Vector{red, green, blue} / 255;
}
