#pragma once

#include "material.h"
#include "shape.h"

class Primitive : public Shape {
 public:
  Primitive() = default;
  Primitive(Material* material);

  Material* material = nullptr;
};
