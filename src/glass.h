#pragma once

#include "config.h"
#include "material.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Glass : public Material {
 public:
  Glass(Vector color);

  Vector outgoingLight(Shape* scene, const TraceRes& hit, Vector direction,
                       real significance) const override;

 private:
  Vector traceColor(Shape* scene, const TraceRes& hit, Vector direction,
                    real significance, real refractive_index) const;

  Vector color_;
};
