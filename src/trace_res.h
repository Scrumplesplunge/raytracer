#pragma once

#include "config.h"
#include "vector.h"

class Primitive;

struct TraceRes {
  TraceRes() = default;
  TraceRes(const Primitive* primitive);

  Vector position, normal;
  const Primitive* primitive;
  real distance;
  bool entering;
};

bool operator<(const TraceRes& left, const TraceRes& right);
