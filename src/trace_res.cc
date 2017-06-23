#include "trace_res.h"
#include "primitive.h"

#include <iostream>

TraceRes::TraceRes(const Primitive* primitive) : primitive(primitive) {}

bool operator<(const TraceRes& left, const TraceRes& right) {
  return left.distance < right.distance;
}
