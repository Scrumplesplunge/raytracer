#include "sphere.h"

#include <cmath>

Sphere::Sphere(Vector p, real r)
    : position(p), squareRadius(r * r) {}

void Sphere::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  /*
    A = d . d := 1 because ray.direction is always unit.

    Given this, the quadratic formula simplifies:
    (-2(o - c) . d +- sqrt(4((o - c) . d)^2 - 4((o - c) . (o - c) - r^2))) / 2
    -> -(o - c) . d +- sqrt(((o - c) . d)^2 + r^2 - (o - c) . (o - c))

    By substituting in v1 = o - c:
    -> -v1 . d +- sqrt((v1 . d)^2 + r^2 - v1 . v1)

    By substituting in s1 = v1 . d:
    -> -s1 +- sqrt(s1^2 + r^2 - v1 . v1);
  */

  Vector rel = ray.start - position;
  real b = dot(rel, ray.direction);
  real det = b * b + squareRadius - dot(rel, rel);

  // No intersection if the determinant is negative.
  if (det < 0) return;

  real sqrtdet = sqrt(det);
  real t1 = -b + sqrtdet;

  // No intersection if the latest intersection is behind the ray origin.
  if (t1 < 0) return;

  // Far intersection.
  TraceRes far(this);

  // Add the distance to the result.
  far.distance = t1;
  far.mask |= TraceRes::DISTANCE;

  // Set the entering flag.
  far.entering = false;
  far.mask |= TraceRes::ENTERING;

  // Conditionally add the position.  We need this to calculate the normal, so
  // we might as well add it if we need that.
  if (ray.mask & (TraceRes::POSITION | TraceRes::NORMAL)) {
    far.position = ray.start + ray.direction * far.distance;
    far.mask |= TraceRes::POSITION;
  }

  if (ray.mask & TraceRes::NORMAL) {
    far.normal = (far.position - position).normalized();
    far.mask |= TraceRes::NORMAL;
  }

  // Near intersection.
  real t2 = -b - sqrtdet;
  if (t2 < 0) {
    output->push_back(far);
    return;
  }

  TraceRes near(this);

  // Add the distance to the result.
  near.distance = t2;
  near.mask |= TraceRes::DISTANCE;

  // Set the entering flag.
  near.entering = true;
  near.mask |= TraceRes::ENTERING;

  // Conditionally add the position.  We need this to calculate the normal, so
  // we might as well add it if we need that.
  if (ray.mask & (TraceRes::POSITION | TraceRes::NORMAL)) {
    near.position = ray.start + ray.direction * near.distance;
    near.mask |= TraceRes::POSITION;
  }

  if (ray.mask & TraceRes::NORMAL) {
    near.normal = (near.position - position).normalized();
    near.mask |= TraceRes::NORMAL;
  }

  // We're done here.
  output->push_back(near);
  output->push_back(far);
}

bool Sphere::Contains(Vector point) const {
  Vector offset = point - position;
  return dot(offset, offset) < squareRadius;
}

const char* Sphere::Name() const { return "Sphere"; }
