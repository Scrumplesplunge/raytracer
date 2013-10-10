#include "Plane.h"

Plane::Plane(const Vector& pos, const Vector& norm) : normal(norm.normalized()), offset(dot(pos, normal)) {}

Array<TraceRes> Plane::trace(const Ray& ray) const {
	Array<TraceRes> out;

	// Component of ray direction in the direction of the surface normal.
	real a = dot(ray.direction, normal);

	// If this is 0, no intersection occurs.
	if (-EPSILON <= a && a <= EPSILON) return out;

	// Component of the ray position in the direction of the surface normal.
	real b = dot(ray.start, normal);

	// offset = a * t + b
	real t = (offset - b) / a;

	// Negative t is not k.
	if (t < 0) return out;

	// We have found the intersection! Hoorah!
	TraceRes hit(this);

	// Add the distance to the result.
	hit.distance = t;
	hit.mask |= TraceRes::DISTANCE;

	// Set the entering flag.
	hit.entering = a < 0;
	hit.mask |= TraceRes::ENTERING;

	// Conditionally add the position.
	if (ray.mask & TraceRes::POSITION) {
		hit.position = ray.start + ray.direction * hit.distance;
		hit.mask |= TraceRes::POSITION;
	}

	// Conditionally add the normal.
	if (ray.mask & TraceRes::NORMAL) {
		hit.normal = normal;
		hit.mask |= TraceRes::NORMAL;
	}

	out.push(hit);
	return out;
}

bool Plane::contains(const Vector& vec) const {
	return dot(vec, normal) < offset;
}

const char *Plane::name() const {
	return "Plane";
}
