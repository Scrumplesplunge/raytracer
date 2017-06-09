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

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;
};
