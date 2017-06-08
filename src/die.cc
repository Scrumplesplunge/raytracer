#include "die.h"

Die::Die(const Matrix &orientation, Material *material)
    : sphere(orientation * Vector(0, 0, 0), SQRT2_2),
      one0_(orientation * Vector(0, 0, -0.675), 0.2),
      two0_(orientation * Vector(0.675, 0.225, -0.225), 0.2),
      two1_(orientation * Vector(0.675, -0.225, 0.225), 0.2),
      three0_(orientation * Vector(0.225, -0.675, -0.225), 0.2),
      three1_(orientation * Vector(0, -0.675, 0), 0.2),
      three2_(orientation * Vector(-0.225, -0.675, 0.225), 0.2),
      four0_(orientation * Vector(-0.225, 0.675, -0.225), 0.2),
      four1_(orientation * Vector(-0.225, 0.675, 0.225), 0.2),
      four2_(orientation * Vector(0.225, 0.675, 0.225), 0.2),
      four3_(orientation * Vector(0.225, 0.675, -0.225), 0.2),
      five0_(orientation * Vector(-0.675, -0.225, -0.225), 0.2),
      five1_(orientation * Vector(-0.675, 0.225, -0.225), 0.2),
      five2_(orientation * Vector(-0.675, 0.225, 0.225), 0.2),
      five3_(orientation * Vector(-0.675, -0.225, 0.225), 0.2),
      five4_(orientation * Vector(-0.675, 0, 0), 0.2),
      six0_(orientation * Vector(0.225, 0.225, 0.675), 0.2),
      six1_(orientation * Vector(0.225, 0, 0.675), 0.2),
      six2_(orientation * Vector(0.225, -0.225, 0.675), 0.2),
      six3_(orientation * Vector(-0.225, 0.225, 0.675), 0.2),
      six4_(orientation * Vector(-0.225, 0, 0.675), 0.2),
      six5_(orientation * Vector(-0.225, -0.225, 0.675), 0.2),
      plane0(orientation * Vector(0, 0, 0.5), orientation(Vector(0, 0, 1))),
      plane1(orientation * Vector(0, 0, -0.5), orientation(Vector(0, 0, -1))),
      plane2(orientation * Vector(0, 0.5, 0), orientation(Vector(0, 1, 0))),
      plane3(orientation * Vector(0, -0.5, 0), orientation(Vector(0, -1, 0))),
      plane4(orientation * Vector(0.5, 0, 0), orientation(Vector(1, 0, 0))),
      plane5(orientation * Vector(-0.5, 0, 0), orientation(Vector(-1, 0, 0))),
      one0(&one0_),
      two0(&two0_),
      two1(&two1_),
      three0(&three0_),
      three1(&three1_),
      three2(&three2_),
      four0(&four0_),
      four1(&four1_),
      four2(&four2_),
      four3(&four3_),
      five0(&five0_),
      five1(&five1_),
      five2(&five2_),
      five3(&five3_),
      five4(&five4_),
      six0(&six0_),
      six1(&six1_),
      six2(&six2_),
      six3(&six3_),
      six4(&six4_),
      six5(&six5_) {
  Primitive *primitives[] = {
      &sphere, &one0_,  &two0_,  &two1_,  &three0_, &three1_, &three2_,
      &four0_, &four1_, &four2_, &four3_, &five0_,  &five1_,  &five2_,
      &five3_, &five4_, &six0_,  &six1_,  &six2_,   &six3_,   &six4_,
      &six5_,  &plane0, &plane1, &plane2, &plane3,  &plane4,  &plane5};
  unsigned int num_primitives = sizeof(primitives) / sizeof(Primitive *);
  for (unsigned int i = 0; i < num_primitives; i++)
    primitives[i]->material = material;

  Shape *intersected[] = {&sphere, &plane0, &plane1, &plane2, &plane3, &plane4,
                          &plane5, &one0,   &two0,   &two1,   &three0, &three1,
                          &three2, &four0,  &four1,  &four2,  &four3,  &five0,
                          &five1,  &five2,  &five3,  &five4,  &six0,   &six1,
                          &six2,   &six3,   &six4,   &six5};
  unsigned int num_intersected = sizeof(intersected) / sizeof(Shape *);
  for (unsigned int i = 0; i < num_intersected; i++) add(intersected[i]);
}
