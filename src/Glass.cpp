#include "Glass.h"
#include "Ray.h"
#include "Primitive.h"
#include <cmath>

Vector Glass::outgoingLight(Shape *scene, const TraceRes& hit, const Vector& direction, real significance) const {
	// Be really lazy if the significance is low enough.
	if (significance < SIGNIFICANCE) return Vector();

	Fresnel children(fresnel(-direction, hit.normal, hit.refractiveIndex1, hit.refractiveIndex2));

	// Output colour.
	Vector output;
	
	// Trace the refractive ray.
	Ray refractiveRay(hit.position - hit.normal * EPSILON, children.refract, TraceRes::ALL);
	Array<TraceRes> res(scene->trace(refractiveRay));
	if (res.length() > 0) {
		Vector temp = res[0].primitive->material->outgoingLight(scene, res[0], -children.refract, significance * (1 - children.weight));
		Vector colorized(temp.x * pow(0.8, res[0].distance), temp.y * pow(0.9, res[0].distance), temp.z * pow(0.9, res[0].distance));
		output = output + colorized * (1 - children.weight);
	}

	// Trace the reflective ray.
	Ray reflectiveRay(hit.position + hit.normal * EPSILON, children.reflect, TraceRes::ALL);
	Array<TraceRes> res2(scene->trace(reflectiveRay));
	if (res.length() > 0) {
		Vector temp = res[0].primitive->material->outgoingLight(scene, res[0], -children.reflect, significance * children.weight);
		Vector colorized(temp.x * 0.8, temp.y * 0.9, temp.z * 0.9);
		output = output + children.weight * colorized;
	}

	// Return the resultant colour.
	return output;
}

real Glass::refractiveIndex() const {
	return 1.52;
}

const char *Glass::name() const {
	return "Glass";
}

