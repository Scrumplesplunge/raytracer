#pragma once

#include "csg.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class CSGUnion : public CSG {
 private:
   std::vector<const Shape *> contents;

 public:
  CSGUnion();

  void add(const Shape *);

  virtual std::vector<TraceRes> trace(const Ray &) const;
  virtual bool contains(Vector) const;
  virtual const char *name() const;

  virtual real minX() const;
  virtual real minY() const;
  virtual real minZ() const;

  virtual real maxX() const;
  virtual real maxY() const;
  virtual real maxZ() const;
};
