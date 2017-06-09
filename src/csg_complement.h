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

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;
};
