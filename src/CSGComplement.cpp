#include "CSGComplement.h"
#include "Config.h"

CSGComplement::CSGComplement(const Shape *shape) : contents(shape) {}

Array<TraceRes> CSGComplement::trace(const Ray& ray) const {
	Ray rayCopy(ray);

	// Required for CSG.
	rayCopy.mask |= TraceRes::ENTERING;

	// Perform the trace.
	Array<TraceRes> out(contents->trace(ray));

	// Take its complement.
	for (unsigned int i = 0; i < out.length(); i++) {
		// Invert this.
		out[i].entering = !out[i].entering;

		// If we have surface normals, invert these too.
		if (rayCopy.mask & TraceRes::NORMAL) out[i].normal = -out[i].normal;
	}

	// Return the result.
	return out;
}

bool CSGComplement::contains(const Vector& vec) const {
	return !contents->contains(vec);
}

const char *CSGComplement::name() const {
	return "CSG Complement";
}

real CSGComplement::minX() const {
	return -INFINITY;
}

real CSGComplement::minY() const {
	return -INFINITY;
}

real CSGComplement::minZ() const {
	return -INFINITY;
}

real CSGComplement::maxX() const {
	return INFINITY;
}

real CSGComplement::maxY() const {
	return INFINITY;
}

real CSGComplement::maxZ() const {
	return INFINITY;
}

