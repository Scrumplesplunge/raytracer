#include "ray.h"

Ray::Ray(const Vector& s, const Vector& d, unsigned int m) : start(s), direction(d.normalized()), mask(m) {}

