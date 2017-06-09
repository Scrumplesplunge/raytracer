#pragma once

#include "vector.h"

class Ray {
 public:
  Vector start, direction;
  unsigned int mask;

  Ray(Vector, Vector, unsigned int);
};
