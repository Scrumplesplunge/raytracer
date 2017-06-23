#pragma once

#include "csg.h"
#include "material.h"
#include "matrix.h"
#include "plane.h"
#include "sphere.h"

class Die : public Intersection {
 public:
  Die(const Matrix& transform, Material* material);

 private:
  class AntiSphere : public Shape {
   public:
    AntiSphere();
    AntiSphere(Vector position, real radius, Material* material);
    AntiSphere& operator=(const AntiSphere& other);

    void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
    bool Contains(Vector point) const override;

   private:
    Sphere sphere_;
    Complement complement_;
  };

  Sphere bounding_sphere_;
  Plane bounding_planes_[6];
  static const int NUM_DOTS = 1 + 2 + 3 + 4 + 5 + 6;
  AntiSphere dots_[NUM_DOTS];
};
