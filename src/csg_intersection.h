#pragma once

#include "csg.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class CSGIntersection : public CSG {
 private:
   std::vector<const Shape *> contents;

 public:
  CSGIntersection();

  void add(const Shape *);

  virtual std::vector<TraceRes> trace(const Ray &) const;
  virtual bool contains(Vector) const;
  virtual const char *name() const;
};
