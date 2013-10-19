#include "Glass.h"
#include "Ray.h"

Vector Glass::outgoingLight(const TraceRes& hit, const Vector& direction) const {
	if (hit.entering) {
		// Generate the refracted ray inside the material.
		Ray newRay(hit.position - hit.normal * EPSILON, refract(-direction, hit.normal, hit.refractiveIndex1, hit.refractiveIndex2), TraceRes::DISTANCE | TraceRes::ENTERING);
		
	} else {

	}
}

real Glass::refractiveIndex() const {
	return 1.52;
}

const char *Glass::name() const {
	return "Glass";
}

