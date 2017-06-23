#pragma once

#include "material.h"
#include "shape.h"

class Primitive : public Shape {
 public:
  Material *material;
};
