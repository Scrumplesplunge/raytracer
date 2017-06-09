#pragma once

#include "config.h"
#include "vector.h"

class Matrix {
 public:
  Vector a, b, c, d;

 public:
  Matrix();
  Matrix(Vector, Vector, Vector, Vector);

  Matrix translate(Vector) const;
  Matrix rotateX(real) const;
  Matrix rotateY(real) const;
  Matrix rotateZ(real) const;

  // Rotate but do not translate.
  Vector operator()(Vector) const;
};

Vector operator*(const Matrix&, Vector);
