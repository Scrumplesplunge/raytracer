#pragma once

#include "vector.h"

struct Ray {
  // Direction must be normalized.
  Vector start, direction;
};
