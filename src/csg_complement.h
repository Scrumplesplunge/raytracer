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

  std::vector<TraceRes> trace(const Ray&) const override;
  bool contains(Vector) const override;
  const char* name() const override;
};
