#include "plane.h"

Plane::Plane(Material* material, Vector position, Vector normal)
    : Primitive{material}, normal_(normal), offset_(dot(position, normal)) {}

void Plane::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  // Component of ray direction in the direction of the surface normal.
  real a = dot(ray.direction, normal_);

  // If this is 0, no intersection occurs.
  if (-EPSILON <= a && a <= EPSILON) return;

  // Component of the ray position in the direction of the surface normal.
  real b = dot(ray.start, normal_);

  // offset = a * t + b
  real t = (offset_ - b) / a;

  // Negative t is not k.
  if (t < 0) return;

  // We have found the intersection! Hoorah!
  TraceRes hit = {material};

  hit.distance = t;
  hit.entering = a < 0;
  hit.position = ray.start + ray.direction * hit.distance;
  hit.normal = normal_;

  output->push_back(hit);
}

bool Plane::Contains(Vector point) const {
  return dot(point, normal_) < offset_;
}
