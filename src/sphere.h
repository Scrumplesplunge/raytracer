#pragma once

#include "config.h"
#include "shape.h"
#include "vector.h"

#include <vector>

class Sphere : public Shape {
 public:
  Sphere() = default;
  Sphere(const Material* material, Vector position, real radius);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  const Material* material_ = nullptr;
  Vector position_ = {0, 0, 0};
  real square_radius_ = 1;
};
