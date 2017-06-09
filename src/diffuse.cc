#include "diffuse.h"
#include "config.h"
#include "primitive.h"
#include "random.h"

#include <cmath>

std::uniform_real_distribution<real> random_real_diffuse(0, 1);

Diffuse::Diffuse(real diff, Vector col) : diffuse(diff), color(col) {}

Vector Diffuse::outgoingLight(Shape* scene, const TraceRes& hit,
                              Vector direction,
                              real significance) const {
  // Be lazy if significance is low enough.
  if (significance < SIGNIFICANCE) return Vector();

  // Generate a random ray.
  Vector norm = hit.entering ? hit.normal : -hit.normal;
  Vector ref = reflect(-direction, norm);
  Vector rnd = Vector::random().normalized();
  Vector vec = (rnd + (1 - EPSILON - diffuse) * ref).normalized();

  real mul = dot(vec, hit.normal);
  if (mul < 0) {
    vec = -vec;
    mul = -mul;
  }

  Ray randomRay(hit.position + norm * EPSILON, vec, TraceRes::ALL);
  std::vector<TraceRes> boundaries = scene->trace(randomRay);

  if (boundaries.size() == 0) return {};

  const Material& material = *boundaries[0].primitive->material;
  Vector light = material.outgoingLight(
      scene, boundaries[0], -vec, significance * mul);
  Vector temp = mul * light;
  return temp * color;
}

const char* Diffuse::name() const { return "Diffuse"; }
