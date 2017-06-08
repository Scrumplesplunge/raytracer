#pragma once

#include "csg.h"
#include "ray.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class CSGComplement : public CSG {
 private:
  const Shape* contents;

 public:
  CSGComplement(const Shape*);

  virtual std::vector<TraceRes> trace(const Ray&) const;
  virtual bool contains(const Vector&) const;
  virtual const char* name() const;

  virtual real minX() const;
  virtual real minY() const;
  virtual real minZ() const;

  virtual real maxX() const;
  virtual real maxY() const;
  virtual real maxZ() const;
};
