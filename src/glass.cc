#include "glass.h"
#include "primitive.h"
#include "random.h"
#include "ray.h"

#include <algorithm>
#include <cmath>
#include <vector>

std::uniform_real_distribution<real> random_real(0, 1);

Glass::Glass(Vector color) : color_(color) {}

Vector Glass::outgoingLight(Shape* scene, const TraceRes& hit,
                            Vector direction, real significance) const {
  // Be really lazy if the significance is low enough.
  if (significance < SIGNIFICANCE) return {};
  Vector red_part = traceColor(scene, hit, direction, significance / 3, 1.51);
  Vector green_part = traceColor(scene, hit, direction, significance / 3, 1.52);
  Vector blue_part = traceColor(scene, hit, direction, significance / 3, 1.53);
  return Vector{1, 0, 0} * red_part +
         Vector{0, 1, 0} * green_part +
         Vector{0, 0, 1} * blue_part;
}

Vector Glass::traceColor(Shape* scene, const TraceRes& hit,
                         Vector direction, real significance,
                         real refractive_index) const {
  Vector norm = hit.entering ? hit.normal : -hit.normal;
  Fresnel children = fresnel(
      -direction, norm, hit.entering ? real{1} : refractive_index,
      hit.entering ? refractive_index : real{1});

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
      return temp * color_;
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
        return temp * Vector{std::pow(color_.x, boundaries[0].distance),
                             std::pow(color_.y, boundaries[0].distance),
                             std::pow(color_.z, boundaries[0].distance)};
      } else {
        return temp;
      }
    }
  }

  return {};
}
