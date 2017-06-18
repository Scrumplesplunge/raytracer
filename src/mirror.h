#pragma once

#include "config.h"
#include "material.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Mirror : public Material {
 public:
  Mirror(Vector color);

  Vector outgoingLight(Shape* scene, const TraceRes& hit, Vector direction,
                       real significance) const override;

  const char *name() const override { return "Mirror"; }

 private:
  Vector color_;
};
