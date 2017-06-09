#pragma once

#include "config.h"
#include "material.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"

class Glass : public Material {
 private:
  Vector color;

 public:
  Glass(Vector);

  Vector outgoingLight(Shape *, const TraceRes &, Vector, real) const override;

  const char *name() const override;
};
