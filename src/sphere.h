#pragma once

#include "config.h"
#include "primitive.h"
#include "vector.h"

#include <vector>

class Sphere : public Primitive {
 public:
  Sphere(Vector position, real radius);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;

 private:
  Vector position_;
  real square_radius_;
};
