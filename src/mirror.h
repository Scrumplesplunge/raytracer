#pragma once

#include "config.h"
#include "material.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Mirror : public Material {
 public:
  Mirror(Vector color);

  Vector OutgoingLight(const Shape* scene, const TraceRes& hit,
                       Vector direction, real significance) const override;
 private:
  Vector color_;
};
