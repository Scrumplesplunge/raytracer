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

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;
};
