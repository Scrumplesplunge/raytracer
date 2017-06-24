#pragma once

#include "csg.h"
#include "material.h"
#include "shapes.h"
#include "transform.h"

class Die : public Intersection {
 public:
  Die(Material* material, const Transform& transform);

 private:
  class AntiSphere : public Shape {
   public:
    AntiSphere();
    AntiSphere(Material* material, Vector position, real radius);

    void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
    bool Contains(Vector point) const override;

   private:
    Complement complement_;
  };

  Sphere bounding_sphere_;
  Plane bounding_planes_[6];
  static const int NUM_DOTS = 1 + 2 + 3 + 4 + 5 + 6;
  AntiSphere dots_[NUM_DOTS];
};
