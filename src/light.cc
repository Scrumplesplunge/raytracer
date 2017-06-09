#include "light.h"

#include <cmath>

Light::Light(Vector col) : color(col) {}

Vector Light::outgoingLight(
    Shape* /* scene */, const TraceRes& /* hit */, Vector /* direction */,
    real /* significance */) const {
  return color;
}

const char* Light::name() const { return "Light Source"; }
