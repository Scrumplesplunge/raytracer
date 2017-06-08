#pragma once

#include "shape.h"
#include "vector.h"

#include <vector>

class KDTree : public Shape {
 private:
  real split;
  char splitDirection;
  std::vector<Shape*> a, b;

 public:
};
