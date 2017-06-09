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

  std::vector<TraceRes> trace(const Ray &) const override;
  bool contains(Vector) const override;
  const char *name() const override;
};
