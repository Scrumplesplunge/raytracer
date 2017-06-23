#pragma once

#include "config.h"
#include "matrix.h"
#include "ray.h"

class Camera {
 public:
  Camera(unsigned int width, unsigned int height, real field_of_view);

  void MoveTo(Vector position);
  void LookAt(Vector position);

  Ray GetRay(real x, real y);

  unsigned int width() const { return width_; }
  unsigned int height() const { return height_; }

 private:
  Matrix transform_;
  unsigned int width_, height_;
  real scale_factor_;
};
