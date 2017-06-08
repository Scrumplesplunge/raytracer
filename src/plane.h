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
  Plane(const Vector &, const Vector &);

  virtual std::vector<TraceRes> trace(const Ray &) const;
  virtual bool contains(const Vector &) const;
  virtual const char *name() const;

  virtual real minX() const;
  virtual real minY() const;
  virtual real minZ() const;

  virtual real maxX() const;
  virtual real maxY() const;
  virtual real maxZ() const;
};
