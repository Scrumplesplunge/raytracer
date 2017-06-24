#include "sphere.h"

#include <cmath>

Sphere::Sphere(const Material* material, Vector position, real radius)
    : material_(material), position_(position),
      square_radius_(radius * radius) {}

void Sphere::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  // Call:
  //
  //     position_      -> p
  //     square_radius_ -> r^2
  //     ray.start      -> s
  //     ray.direction  -> d
  //
  // Points on the ray can be described by:
  //
  //     x = s + td
  //
  // And points on the surface of the sphere can be described by:
  //
  //     |x - p| = r
  //
  // So intersection points occur when:
  //
  //     |s + td - p| = r
  //  => |(s - p) + td| = r
  //  => ((s - p) + td)^2 = r^2
  //  => (s - p)^2 + 2t(d.(s - p)) + t^2(d^2) = r^2
  //  => (d^2)t^2 + 2d(s - p)t + (s - p)^2 - r^2 = 0
  //
  // So we have a quadratic At^2 + Bt + C = 0 with:
  //
  //     A = d^2 = dot(ray.direction, ray.direction) = 1
  //     B = 2d(s - p)
  //     C = (s - p)^2 - r^2
  //
  // The quadratic formula can be simplified slightly with these values.
  //
  //     t = (-B +- sqrt(B^2 - 4AC)) / 2A
  //  => t = (-B +- sqrt(B^2 - 4C)) / 2       (A = 1)
  //  => t = (-2D +- sqrt(4D^2 - 4C)) / 2     (B = 2D)
  //  => t = -D +- sqrt(D^2 - C)
  Vector offset = ray.start - position_;
  real d = dot(offset, ray.direction);
  real c = dot(offset, offset) - square_radius_;

  // If the determinant would be negative, there will be no roots.
  if (d * d < c) return;
  real root_determinant = sqrt(d * d - c);
  real t1 = -d - root_determinant;
  real t2 = -d + root_determinant;

  if (t1 >= 0) {
    TraceRes near = {material_};
    near.distance = t1;
    near.entering = true;
    near.position = ray.start + ray.direction * near.distance;
    near.normal = (near.position - position_).normalized();
    output->push_back(near);
  }
  if (t2 >= 0) {
    TraceRes far = {material_};
    far.distance = t2;
    far.entering = false;
    far.position = ray.start + ray.direction * far.distance;
    far.normal = (far.position - position_).normalized();
    output->push_back(far);
  }
}

bool Sphere::Contains(Vector point) const {
  Vector offset = point - position_;
  return dot(offset, offset) < square_radius_;
}
