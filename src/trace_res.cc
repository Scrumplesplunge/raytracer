#include "trace_res.h"

#include <iostream>

TraceRes::TraceRes(const Material* material) : material(material) {}

bool operator<(const TraceRes& left, const TraceRes& right) {
  return left.distance < right.distance;
}
