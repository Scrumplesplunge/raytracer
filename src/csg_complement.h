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
  virtual bool contains(Vector) const;
  virtual const char* name() const;
};
