#include "transform.h"

#include <cmath>

Transform Transform::Translate(Vector offset) const {
  return {a_, b_, c_, d_ + offset};
}

Transform Transform::RotateX(real radians) const {
  real ca = cos(radians), sa = sin(radians);
  Vector q = {0, ca, -sa}, r = {0, sa, ca};
  return {{a_.x, dot(q, a_), dot(r, a_)},
          {b_.x, dot(q, b_), dot(r, b_)},
          {c_.x, dot(q, c_), dot(r, c_)},
          {d_.x, dot(q, d_), dot(r, d_)}};
}

Transform Transform::RotateY(real radians) const {
  real ca = cos(radians), sa = sin(radians);
  Vector p = {ca, 0, -sa}, r = {sa, 0, ca};
  return {{dot(p, a_), a_.y, dot(r, a_)},
          {dot(p, b_), b_.y, dot(r, b_)},
          {dot(p, c_), c_.y, dot(r, c_)},
          {dot(p, d_), d_.y, dot(r, d_)}};
}

Transform Transform::RotateZ(real radians) const {
  real ca = cos(radians), sa = sin(radians);
  Vector p = {ca, -sa, 0}, q = {sa, ca, 0};
  return {{dot(p, a_), dot(q, a_), a_.z},
          {dot(p, b_), dot(q, b_), b_.z},
          {dot(p, c_), dot(q, c_), c_.z},
          {dot(p, d_), dot(q, d_), d_.z}};
}

Vector Transform::operator()(Vector vector) const {
  return vector.x * a_ + vector.y * b_ + vector.z * c_;
}

Transform::Transform(Vector forward, Vector left, Vector up, Vector offset)
    : a_(forward), b_(left), c_(up), d_(offset) {}

Vector operator*(const Transform& transform, Vector vector) {
  return vector.x * transform.a_ +
         vector.y * transform.b_ +
         vector.z * transform.c_ +
         transform.d_;
}
