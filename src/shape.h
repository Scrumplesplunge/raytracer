#pragma once

#include "ray.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class Shape {
 public:
  virtual std::vector<TraceRes> trace(const Ray &) const = 0;
  virtual bool contains(Vector) const = 0;
  virtual const char *name() const;
};
