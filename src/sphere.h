#pragma once

#include "config.h"
#include "primitive.h"
#include "vector.h"

#include <vector>

class Sphere : public Primitive {
 private:
  Vector position;
  real radius, squareRadius;

 public:
  Sphere(Vector, real);

  std::vector<TraceRes> trace(const Ray &) const override;
  bool contains(Vector) const override;
  const char *name() const override;
};
