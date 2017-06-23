#pragma once

#include "config.h"
#include "vector.h"

class Transform {
 public:
  Transform() = default;

  Transform Translate(Vector offset) const;
  Transform RotateX(real radians) const;
  Transform RotateY(real radians) const;
  Transform RotateZ(real radians) const;

  // Rotate but do not translate.
  Vector operator()(Vector direction) const;

  Vector forward() const { return a_; }
  Vector left() const { return b_; }
  Vector up() const { return c_; }
  Vector offset() const { return d_; }

 private:
  friend class Camera;
  friend Vector operator*(const Transform& transform, Vector position);

  Transform(Vector forward, Vector left, Vector up, Vector offset);

  // These vectors are columns of the matrix. The abstract matrix is of the
  // form:
  // | a.x b.x c.x d.x |
  // | a.y b.y c.y d.y |
  // | a.z b.z c.z d.z |
  // |  0   0   0   1  |
  Vector a_ = {1, 0, 0},
         b_ = {0, 1, 0},
         c_ = {0, 0, 1},
         d_ = {0, 0, 0};
};

Vector operator*(const Transform& transform, Vector position);
