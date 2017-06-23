#include "plane.h"

Plane::Plane(Vector pos, Vector norm)
    : normal(norm.normalized()), offset(dot(pos, normal)) {}

void Plane::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  // Component of ray direction in the direction of the surface normal.
  real a = dot(ray.direction, normal);

  // If this is 0, no intersection occurs.
  if (-EPSILON <= a && a <= EPSILON) return;

  // Component of the ray position in the direction of the surface normal.
  real b = dot(ray.start, normal);

  // offset = a * t + b
  real t = (offset - b) / a;

  // Negative t is not k.
  if (t < 0) return;

  // We have found the intersection! Hoorah!
  TraceRes hit(this);

  hit.distance = t;
  hit.entering = a < 0;
  hit.position = ray.start + ray.direction * hit.distance;
  hit.normal = normal;

  output->push_back(hit);
}

bool Plane::Contains(Vector point) const {
  return dot(point, normal) < offset;
}
