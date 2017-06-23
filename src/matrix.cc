#include "matrix.h"

#include <cmath>

Matrix Matrix::translate(Vector offset) const {
  return {a, b, c, d + offset};
}

Matrix Matrix::rotateX(real radians) const {
  real ca = cos(radians), sa = sin(radians);
  Vector q = {0, ca, -sa}, r = {0, sa, ca};
  return {{a.x, dot(q, a), dot(r, a)},
          {b.x, dot(q, b), dot(r, b)},
          {c.x, dot(q, c), dot(r, c)},
          {d.x, dot(q, d), dot(r, d)}};
}

Matrix Matrix::rotateY(real radians) const {
  real ca = cos(radians), sa = sin(radians);
  Vector p = {ca, 0, -sa}, r = {sa, 0, ca};
  return {{dot(p, a), a.y, dot(r, a)},
          {dot(p, b), b.y, dot(r, b)},
          {dot(p, c), c.y, dot(r, c)},
          {dot(p, d), d.y, dot(r, d)}};
}

Matrix Matrix::rotateZ(real radians) const {
  real ca = cos(radians), sa = sin(radians);
  Vector p = {ca, -sa, 0}, q = {sa, ca, 0};
  return {{dot(p, a), dot(q, a), a.z},
          {dot(p, b), dot(q, b), b.z},
          {dot(p, c), dot(q, c), c.z},
          {dot(p, d), dot(q, d), d.z}};
}

Vector Matrix::operator()(Vector vector) const {
  return vector.x * a + vector.y * b + vector.z * c;
}

Vector operator*(const Matrix& matrix, Vector vector) {
  return vector.x * matrix.a +
         vector.y * matrix.b +
         vector.z * matrix.c +
         matrix.d;
}
