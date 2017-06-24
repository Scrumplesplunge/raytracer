#pragma once

#include "config.h"
#include "shape.h"
#include "vector.h"

#include <vector>

class Plane : public Shape {
 public:
  Plane() = default;

  // The normal should be of unit length.
  Plane(const Material* material, Vector position, Vector normal);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  const Material* material_ = nullptr;
  Vector normal_ = {1, 0, 0};
  real offset_ = 0;
};
