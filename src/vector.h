#pragma once

#include "config.h"

struct Vector {
  real x = {},
       y = {},
       z = {};

  real length() const;
  Vector normalized() const;
  static Vector random();
};

#define MAP_OPERATOR(op)  \
  Vector operator op(Vector a, Vector b);  \
  Vector operator op(real a, Vector b);  \
  Vector operator op(Vector a, real b)
MAP_OPERATOR(+);
MAP_OPERATOR(-);
MAP_OPERATOR(*);
MAP_OPERATOR(/);
#undef MAP_OPERATOR

Vector operator-(Vector v);
real dot(Vector a, Vector b);
Vector cross(Vector a, Vector b);
