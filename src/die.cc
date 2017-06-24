#include "die.h"

namespace {

struct SphereParameters {
  Vector position;
  real radius;
};

struct PlaneParameters {
  Vector position;
  Vector normal;
};

SphereParameters bounding_sphere{{0, 0, 0}, SQRT2_2};
PlaneParameters bounding_planes[] = {
  {{0, 0, 0.5}, {0, 0, 1}},
  {{0, 0, -0.5}, {0, 0, -1}},
  {{0, 0.5, 0}, {0, 1, 0}},
  {{0, -0.5, 0}, {0, -1, 0}},
  {{0.5, 0, 0}, {1, 0, 0}},
  {{-0.5, 0, 0}, {-1, 0, 0}},
};
SphereParameters dots[] = {
  {{0, 0, -0.675}, 0.2},
  {{0.675, 0.225, -0.225}, 0.2},
  {{0.675, -0.225, 0.225}, 0.2},
  {{0.225, -0.675, -0.225}, 0.2},
  {{0, -0.675, 0}, 0.2},
  {{-0.225, -0.675, 0.225}, 0.2},
  {{-0.225, 0.675, -0.225}, 0.2},
  {{-0.225, 0.675, 0.225}, 0.2},
  {{0.225, 0.675, 0.225}, 0.2},
  {{0.225, 0.675, -0.225}, 0.2},
  {{-0.675, -0.225, -0.225}, 0.2},
  {{-0.675, 0.225, -0.225}, 0.2},
  {{-0.675, 0.225, 0.225}, 0.2},
  {{-0.675, -0.225, 0.225}, 0.2},
  {{-0.675, 0, 0}, 0.2},
  {{0.225, 0.225, 0.675}, 0.2},
  {{0.225, 0, 0.675}, 0.2},
  {{0.225, -0.225, 0.675}, 0.2},
  {{-0.225, 0.225, 0.675}, 0.2},
  {{-0.225, 0, 0.675}, 0.2},
  {{-0.225, -0.225, 0.675}, 0.2},
};

}  // namespace

Die::Die(Material* material, const Transform& transform) {
  Add(std::make_unique<Sphere>(material, transform * bounding_sphere.position,
                               bounding_sphere.radius));
  for (int i = 0; i < 6; i++) {
    Add(std::make_unique<Plane>(material,
                                transform * bounding_planes[i].position,
                                transform(bounding_planes[i].normal)));
  }
  for (int i = 0; i < NUM_DOTS; i++) {
    Add(std::make_unique<AntiSphere>(material, transform * dots[i].position,
                                     dots[i].radius));
  }
}

Die::AntiSphere::AntiSphere() : complement_(std::make_unique<Sphere>()) {}

Die::AntiSphere::AntiSphere(Material* material, Vector position, real radius)
    : complement_(std::make_unique<Sphere>(material, position, radius)) {}

void Die::AntiSphere::Trace(
    const Ray& ray, std::vector<TraceRes>* output) const {
  return complement_.Trace(ray, output);
}

bool Die::AntiSphere::Contains(Vector point) const {
  return complement_.Contains(point);
}
