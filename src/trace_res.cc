#include "trace_res.h"
#include "primitive.h"

#include <iostream>

TraceRes::TraceRes() : mask(0) {}

TraceRes::TraceRes(const Primitive *prim)
    : mask(TraceRes::PRIMITIVE), primitive(prim) {}

bool operator<(const TraceRes& left, const TraceRes& right) {
  return left.distance < right.distance;
}

void TraceRes::print() {
  std::cout << "TraceRes on object " << primitive->name() << " @[" << primitive
            << "]";
  if (mask & TraceRes::ENTERING)
    std::cout << " (" << (entering ? "ENTERING" : "EXITING") << ")";
  if (mask & TraceRes::DISTANCE) std::cout << " DIST=" << distance;
  std::cout << std::endl;
}
