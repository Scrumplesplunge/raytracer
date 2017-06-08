#include "light.h"

#include <cmath>

Light::Light(const Vector& col) : color(col) {}

Vector Light::outgoingLight(Shape *scene, const TraceRes& hit, const Vector& direction, real significance) const {
	return color;
}

const char *Light::name() const {
	return "Light Source";
}

