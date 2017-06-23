#pragma once

#include "config.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Material {
 public:
  virtual Vector OutgoingLight(const Shape* scene, const TraceRes& hit,
                               Vector direction, real significance) const = 0;

  struct Fresnel {
    real fraction_reflected;
    Vector reflect, refract;
  };

  static Vector Refract(Vector vector, Vector normal, real refractive_index_1,
                        real refractive_index_2);
  static Vector Reflect(Vector vector, Vector normal);
  static Fresnel ComputeFresnel(
      Vector vector, Vector normal, real refractive_index_1,
      real refractive_index_2);
};
