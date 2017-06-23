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

Die::Die(const Matrix& transform, Material* material) {
  bounding_sphere_ =
      {transform * bounding_sphere.position, bounding_sphere.radius};
  bounding_sphere_.material = material;
  Add(&bounding_sphere_);
  for (int i = 0; i < 6; i++) {
    bounding_planes_[i] = {transform * bounding_planes[i].position,
                           transform(bounding_planes[i].normal)};
    bounding_planes_[i].material = material;
    Add(&bounding_planes_[i]);
  }
  for (int i = 0; i < NUM_DOTS; i++) {
    dots_[i] = {transform * dots[i].position, dots[i].radius, material};
    Add(&dots_[i]);
  }
}

Die::AntiSphere::AntiSphere() : complement_(&sphere_) {}

Die::AntiSphere::AntiSphere(Vector position, real radius, Material* material)
    : sphere_(position, radius), complement_(&sphere_) {
  sphere_.material = material;
}

Die::AntiSphere& Die::AntiSphere::operator=(const AntiSphere& other) {
  // Don't copy the complement_; the pointers are already correct so just update
  // the sphere being pointed to.
  sphere_ = other.sphere_;
  return *this;
}

void Die::AntiSphere::Trace(
    const Ray& ray, std::vector<TraceRes>* output) const {
  return complement_.Trace(ray, output);
}

bool Die::AntiSphere::Contains(Vector point) const {
  return complement_.Contains(point);
}
