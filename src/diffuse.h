#pragma once

#include "config.h"
#include "material.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Diffuse : public Material {
 public:
  // The diffuse factor should be in the range [0-1]. When the factor is 0, the
  // surface is perfectly reflective. When the factor is 1, the surface is
  // perfectly diffused.
  Diffuse(real diffuse, Vector color);

  Vector OutgoingLight(const Shape* scene, const TraceRes& hit,
                       Vector direction, real significance) const override;

 private:
  real diffuse_;
  Vector color_;
};
