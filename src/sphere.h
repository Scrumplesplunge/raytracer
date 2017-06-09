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

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;
};
