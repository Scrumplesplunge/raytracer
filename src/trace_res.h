#pragma once

#include "config.h"
#include "vector.h"

class Material;

struct TraceRes {
  TraceRes() = default;
  TraceRes(const Material* material);

  Vector position, normal;
  const Material* material;
  real distance;
  bool entering;
};

bool operator<(const TraceRes& left, const TraceRes& right);
