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

  virtual std::vector<TraceRes> trace(const Ray &) const;
  virtual bool contains(Vector) const;
  virtual const char *name() const;
};
