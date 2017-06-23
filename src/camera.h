#pragma once

#include "config.h"
#include "matrix.h"
#include "ray.h"

class Camera {
 public:
  Camera(int width, int height, real field_of_view);

  void MoveTo(Vector position);
  void LookAt(Vector position);

  Ray GetRay(real x, real y);

  int width() const { return width_; }
  int height() const { return height_; }

 private:
  Matrix transform_;
  int width_, height_;
  real scale_factor_;
};
