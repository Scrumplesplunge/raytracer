#include "mirror.h"
#include "primitive.h"
#include "random.h"

Mirror::Mirror(Vector color) : color_(color) {}

Vector Mirror::OutgoingLight(const Shape* scene, const TraceRes& hit,
                             Vector direction, real significance) const {
  if (significance < EPSILON) return {};

  // Take the colour of the reflected ray.
  Vector normal = hit.entering ? hit.normal : -hit.normal;
  Vector reflected_direction = Reflect(-direction, normal);
  Ray reflected_ray{hit.position + normal * EPSILON, reflected_direction};
  std::vector<TraceRes> boundaries;
  scene->Trace(reflected_ray, &boundaries);
  
  if (boundaries.size() == 0) return {};

  Vector light = boundaries[0].material->OutgoingLight(
      scene, boundaries[0], -reflected_direction, significance * 0.9);
  return light * color_;
}
