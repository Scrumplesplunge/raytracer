#include "glass.h"
#include "primitive.h"
#include "random.h"
#include "ray.h"

#include <algorithm>
#include <cmath>
#include <vector>

std::uniform_real_distribution<real> random_real(0, 1);

Glass::Glass(Vector col) : color(col) {}

Vector Glass::outgoingLight(Shape* scene, const TraceRes& hit,
                            Vector direction, real significance) const {
  // Be really lazy if the significance is low enough.
  if (significance < SIGNIFICANCE) return {};

  Vector norm = hit.entering ? hit.normal : -hit.normal;

  Fresnel children = fresnel(-direction, norm, hit.entering ? 1 : 1.53,
                             hit.entering ? 1.53 : 1);

  real rand = random_real(RandomGenerator());
  if (rand <= children.weight) {
    // Trace the reflective ray.
    Ray reflectiveRay(hit.position + norm * EPSILON, children.reflect,
                      TraceRes::ALL);
    std::vector<TraceRes> boundaries;
    scene->Trace(reflectiveRay, &boundaries);
    if (boundaries.size() > 0) {
      real mul = children.weight;
      Vector temp = boundaries[0].primitive->material->outgoingLight(
          scene, boundaries[0], -children.reflect, significance * mul);
      return temp * color;
    }
  } else {
    // Trace the refractive ray.
    Ray refractiveRay(hit.position - norm * EPSILON, children.refract,
                      TraceRes::ALL);
    std::vector<TraceRes> boundaries;
    scene->Trace(refractiveRay, &boundaries);
    if (boundaries.size() > 0) {
      real mul = (1 - children.weight);
      Vector temp = boundaries[0].primitive->material->outgoingLight(
          scene, boundaries[0], -children.refract, significance * mul);
      if (hit.entering) {
        return temp * Vector{std::pow(color.x, boundaries[0].distance),
                             std::pow(color.y, boundaries[0].distance),
                             std::pow(color.z, boundaries[0].distance)};
      } else {
        return temp;
      }
    }
  }

  return {};
}

const char* Glass::name() const { return "Glass"; }
