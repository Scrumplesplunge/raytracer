#pragma once

#include "ray.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class Shape {
 public:
  virtual std::vector<TraceRes> trace(const Ray &) const = 0;
  virtual bool contains(const Vector &) const = 0;
  virtual const char *name() const;

  virtual real minX() const = 0;
  virtual real minY() const = 0;
  virtual real minZ() const = 0;

  virtual real maxX() const = 0;
  virtual real maxY() const = 0;
  virtual real maxZ() const = 0;
};
