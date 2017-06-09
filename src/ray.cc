#include "ray.h"

Ray::Ray(Vector s, Vector d, unsigned int m)
    : start(s), direction(d.normalized()), mask(m) {}
