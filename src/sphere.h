#pragma once

#include "config.h"
#include "primitive.h"
#include "vector.h"

#include <vector>

class Sphere : public Primitive {
 private:
  Vector position;
  real squareRadius;

 public:
  Sphere(Vector, real);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;
};
