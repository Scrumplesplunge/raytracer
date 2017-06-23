#include "diffuse.h"
#include "config.h"
#include "primitive.h"
#include "random.h"

#include <cmath>

std::uniform_real_distribution<real> random_real_diffuse(0, 1);

Diffuse::Diffuse(real diffuse, Vector color)
    : diffuse_(diffuse), color_(color) {}

Vector Diffuse::outgoingLight(const Shape* scene, const TraceRes& hit,
                              Vector direction, real significance) const {
  // Be lazy if significance is low enough.
  if (significance < SIGNIFICANCE) return {};

  // Generate a random ray.
  Vector normal = hit.entering ? hit.normal : -hit.normal;
  Vector random = Vector::random().normalized();
  if (dot(random, normal) < 0) random = -random;
  Vector reflected = reflect(-direction, normal);
  Vector vector = (random * diffuse_ + reflected * (1 - diffuse_)).normalized();

  Ray randomRay{hit.position + normal * EPSILON, vector};
  std::vector<TraceRes> boundaries;
  scene->Trace(randomRay, &boundaries);

  if (boundaries.size() == 0) return {};

  const Material& material = *boundaries[0].primitive->material;
  real light_significance = dot(vector, normal);
  Vector light = material.outgoingLight(
      scene, boundaries[0], -vector, significance * light_significance);
  return light_significance * light * color_;
}
