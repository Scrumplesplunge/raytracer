#include "light.h"

#include <cmath>

Light::Light(Vector color) : color_(color) {}

Vector Light::outgoingLight(
    const Shape* /* scene */, const TraceRes& /* hit */, Vector /* direction */,
    real /* significance */) const {
  return color_;
}
