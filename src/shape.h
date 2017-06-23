#pragma once

#include "ray.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class Shape {
 public:
  virtual void Trace(const Ray& ray, std::vector<TraceRes>* output) const = 0;
  virtual bool Contains(Vector point) const = 0;
};
