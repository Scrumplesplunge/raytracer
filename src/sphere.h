#pragma once

#include "config.h"
#include "primitive.h"
#include "vector.h"

#include <vector>

class Sphere : public Primitive {
 public:
  Sphere() = default;
  Sphere(Material* material, Vector position, real radius);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  Vector position_ = {0, 0, 0};
  real square_radius_ = 1;
};
