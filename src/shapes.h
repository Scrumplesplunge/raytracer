#pragma once

#include "config.h"
#include "ray.h"
#include "trace_res.h"
#include "vector.h"

#include <vector>

class Shape {
 public:
  virtual void Trace(const Ray& ray, std::vector<TraceRes>* output) const = 0;
  virtual bool Contains(Vector point) const = 0;
};

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
