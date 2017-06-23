#pragma once

#include "image.h"
#include "material.h"

class Sky : public Material, public Image {
 public:
  Sky(const char* filename);

  Vector OutgoingLight(const Shape* scene, const TraceRes& hit,
                       Vector direction, real significance) const override;
};
