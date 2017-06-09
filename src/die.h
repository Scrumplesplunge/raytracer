#pragma once

#include "csg.h"
#include "material.h"
#include "matrix.h"
#include "plane.h"
#include "sphere.h"

class Die : public Intersection {
 private:
  Sphere sphere, one0_, two0_, two1_, three0_, three1_, three2_, four0_, four1_,
      four2_, four3_, five0_, five1_, five2_, five3_, five4_, six0_, six1_,
      six2_, six3_, six4_, six5_;
  Plane plane0, plane1, plane2, plane3, plane4, plane5;
  Complement one0, two0, two1, three0, three1, three2, four0, four1, four2,
      four3, five0, five1, five2, five3, five4, six0, six1, six2, six3, six4,
      six5;

 public:
  Die(const Matrix&, Material*);
};
