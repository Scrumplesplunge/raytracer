#include "AABB.h"

AABB::AABB(const Vector& a, const Vector& b) : min(a), max(b) {}

bool AABB::obstructs(const Ray& ray) const {

}

