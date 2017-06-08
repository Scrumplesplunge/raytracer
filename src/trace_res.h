#pragma once

#include "config.h"
#include "vector.h"

class Primitive;

class TraceRes {
 public:
  unsigned int mask;
  const Primitive *primitive;
  Vector position, normal;
  real distance;
  bool entering;

  static const int PRIMITIVE = 0x00000001, POSITION = 0x00000002,
                   NORMAL = 0x00000004, DISTANCE = 0x00000008,
                   ENTERING = 0x00000010, ALL = 0xffffffff;

 public:
  TraceRes();
  TraceRes(const Primitive *);

  void print();
};

bool operator<(const TraceRes& left, const TraceRes& right);
