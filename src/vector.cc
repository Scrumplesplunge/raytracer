#include "vector.h"

#include "random.h"

real Vector::length() const {
  return sqrt(dot(*this, *this));
}

Vector Vector::normalized() const {
  return *this / length();
}

Vector Vector::random() {
  std::normal_distribution<real> random(0, 1);
  return {random(RandomGenerator()),
          random(RandomGenerator()),
          random(RandomGenerator())};
}

#define MAP_OPERATOR(op)  \
  Vector operator op(Vector a, Vector b) {  \
    return {a.x op b.x,  \
            a.y op b.y,  \
            a.z op b.z};  \
  }  \
  Vector operator op(real a, Vector b) {  \
    return {a op b.x,  \
            a op b.y,  \
            a op b.z};  \
  }  \
  Vector operator op(Vector a, real b) {  \
    return {a.x op b,  \
            a.y op b,  \
            a.z op b};  \
  }
MAP_OPERATOR(+)
MAP_OPERATOR(-)
MAP_OPERATOR(*)
MAP_OPERATOR(/)
#undef MAP_OPERATOR

Vector operator-(Vector v) {
  return {-v.x, -v.y, -v.z};
}

real dot(Vector a, Vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector cross(Vector a, Vector b) {
  return {a.y * b.z - a.z * b.y,
          a.z * b.x - a.x * b.z,
          a.x * b.y - a.y * b.x};
}
