#pragma once

#include "config.h"
#include "vector.h"

struct Matrix {
  Matrix translate(Vector offset) const;
  Matrix rotateX(real radians) const;
  Matrix rotateY(real radians) const;
  Matrix rotateZ(real radians) const;

  // Rotate but do not translate.
  Vector operator()(Vector direction) const;

  // These vectors are columns of the matrix. The abstract matrix is of the
  // form:
  // | a.x b.x c.x d.x |
  // | a.y b.y c.y d.y |
  // | a.z b.z c.z d.z |
  // |  0   0   0   1  |
  Vector a = {1, 0, 0},
         b = {0, 1, 0},
         c = {0, 0, 1},
         d = {0, 0, 0};
};

Vector operator*(const Matrix& matrix, Vector position);
