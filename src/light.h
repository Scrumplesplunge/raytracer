#pragma once

#include "material.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Light : public Material {
 public:
  Light(Vector color);

  Vector outgoingLight(const Shape* scene, const TraceRes& hit,
                       Vector direction, real significance) const override;

 private:
  Vector color_;
};
