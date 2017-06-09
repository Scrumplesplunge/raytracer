#pragma once

#include "config.h"
#include "primitive.h"
#include "vector.h"

#include <vector>

class Plane : public Primitive {
 private:
  Vector normal;
  real offset;

 public:
  Plane(Vector, Vector);

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;
};
