#pragma once

#include "ray.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class Shape {
 public:
  virtual std::vector<TraceRes> Trace(const Ray& ray) const = 0;
  virtual bool Contains(Vector point) const = 0;
  virtual const char* Name() const;
};
