#include "trace_res.h"
#include "primitive.h"

#include <iostream>

TraceRes::TraceRes() : mask(0) {}

TraceRes::TraceRes(const Primitive *prim)
    : mask(TraceRes::PRIMITIVE), primitive(prim) {}

bool operator<(const TraceRes& left, const TraceRes& right) {
  return left.distance < right.distance;
}
