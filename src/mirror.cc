#include "mirror.h"
#include "primitive.h"
#include "random.h"

Mirror::Mirror(Vector color) : color_(color) {}

Vector Mirror::outgoingLight(const Shape* scene, const TraceRes& hit,
                             Vector direction, real significance) const {
  if (significance < EPSILON) return {};

  // Take the colour of the reflected ray.
  Vector normal = hit.entering ? hit.normal : -hit.normal;
  Vector reflected_direction = reflect(-direction, normal);
  Ray reflected_ray{hit.position + normal * EPSILON, reflected_direction};
  std::vector<TraceRes> boundaries;
  scene->Trace(reflected_ray, &boundaries);
  
  if (boundaries.size() == 0) return {};

  const Material& material = *boundaries[0].primitive->material;
  Vector light = material.outgoingLight(
      scene, boundaries[0], -reflected_direction, significance * 0.9);
  return light * color_;
}
