#include "sky.h"

#include <cmath>

Sky::Sky(const char *filename) : Image(filename) {}

Vector Sky::outgoingLight(Shape *scene, const TraceRes &hit,
                          Vector direction, real significance) const {
  Vector rayDir = -direction;

  real v = acos(rayDir.z) / PI;
  Vector tmp = Vector{rayDir.x, rayDir.y, 0}.normalized();
  real u = tmp.x > 0 ? acos(tmp.y) / TWOPI : 1 - acos(tmp.y) / TWOPI;

  unsigned char *pix = operator()(
      ((unsigned int)(u * (getWidth() - 1))) % getWidth(),
      ((unsigned int)(v * (getHeight() - 1))) % getHeight());
  return Vector{pix[2], pix[1], pix[0]} / 255;
}

const char *Sky::name() const { return "Sky Sphere"; }
