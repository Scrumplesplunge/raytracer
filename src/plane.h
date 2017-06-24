#pragma once

#include "config.h"
#include "primitive.h"
#include "vector.h"

#include <vector>

class Plane : public Primitive {
 public:
  Plane() = default;

  // The normal should be of unit length.
  Plane(Material* material, Vector position, Vector normal);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  Vector normal_ = {1, 0, 0};
  real offset_ = 0;
};
