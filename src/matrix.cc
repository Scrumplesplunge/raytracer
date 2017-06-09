#include "matrix.h"

#include <cmath>

Matrix::Matrix() : a{1, 0, 0}, b{0, 1, 0}, c{0, 0, 1}, d{0, 0, 0} {}

Matrix::Matrix(Vector p, Vector q, Vector r, Vector s)
    : a(p), b(q), c(r), d(s) {}

Matrix Matrix::translate(Vector t) const {
  return Matrix(a, b, c, d + t);
}

Matrix Matrix::rotateX(real ang) const {
  real ca = cos(ang), sa = sin(ang);
  Vector q = {0, ca, -sa}, r = {0, sa, ca};
  return Matrix({a.x, dot(q, a), dot(r, a)},
                {b.x, dot(q, b), dot(r, b)},
                {c.x, dot(q, c), dot(r, c)},
                {d.x, dot(q, d), dot(r, d)});
}

Matrix Matrix::rotateY(real ang) const {
  real ca = cos(ang), sa = sin(ang);
  Vector p = {ca, 0, -sa}, r = {sa, 0, ca};
  return Matrix({dot(p, a), a.y, dot(r, a)},
                {dot(p, b), b.y, dot(r, b)},
                {dot(p, c), c.y, dot(r, c)},
                {dot(p, d), d.y, dot(r, d)});
}

Matrix Matrix::rotateZ(real ang) const {
  real ca = cos(ang), sa = sin(ang);
  Vector p = {ca, -sa, 0}, q = {sa, ca, 0};
  return Matrix({dot(p, a), dot(q, a), a.z},
                {dot(p, b), dot(q, b), b.z},
                {dot(p, c), dot(q, c), c.z},
                {dot(p, d), dot(q, d), d.z});
}

Vector Matrix::operator()(Vector v) const {
  return {a.x * v.x + b.x * v.y + c.x * v.z,
          a.y * v.x + b.y * v.y + c.y * v.z,
          a.z * v.x + b.z * v.y + c.z * v.z};
}

Vector operator*(const Matrix& m, Vector v) {
  return {m.a.x * v.x + m.b.x * v.y + m.c.x * v.z + m.d.x,
          m.a.y * v.x + m.b.y * v.y + m.c.y * v.z + m.d.y,
          m.a.z * v.x + m.b.z * v.y + m.c.z * v.z + m.d.z};
}
