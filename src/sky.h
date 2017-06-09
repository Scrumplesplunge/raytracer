#pragma once

#include "image.h"
#include "material.h"

class Sky : public Material, public Image {
 public:
  Sky(const char *);

  Vector outgoingLight(Shape *, const TraceRes &, Vector, real) const override;

  const char *name() const override;
};
