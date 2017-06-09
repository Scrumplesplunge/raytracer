#pragma once

#include "config.h"
#include "matrix.h"
#include "ray.h"

class Camera {
 private:
  Matrix transform;
  unsigned int width, height;
  real mul;

 public:
  Camera(unsigned int, unsigned int, real);

  void moveTo(Vector);
  void lookAt(Vector);

  unsigned int getWidth() const;
  unsigned int getHeight() const;

  Ray getRay(real, real, unsigned int);
};
