#pragma once

#include "shape.h"
#include "vector.h"

class KDTree : public Shape {
 private:
  real split;
  char splitDirection;
  Array<Shape*> a, b;

 public:
};
