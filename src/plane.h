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

  std::vector<TraceRes> trace(const Ray &) const override;
  bool contains(Vector) const override;
  const char *name() const override;
};
