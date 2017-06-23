#pragma once

#include "config.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Material {
 protected:
  struct Fresnel {
    real weight;
    Vector reflect, refract;
  };

  static Vector refract(Vector, Vector, real, real);
  static Vector reflect(Vector, Vector);
  static Fresnel fresnel(Vector, Vector, real, real);

 public:
  virtual Vector outgoingLight(Shape *, const TraceRes &, Vector,
                               real) const = 0;
};
