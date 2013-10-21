#include "Glass.h"
#include "Ray.h"
#include "Primitive.h"
#include <cmath>
#include "Random.h"

std::uniform_real_distribution<real> random_real(0, 1);

Glass::Glass(const Vector& col) : color(col) {}

Vector Glass::outgoingLight(Shape *scene, const TraceRes& hit, const Vector& direction, real significance) const {
	// Be really lazy if the significance is low enough.
	if (significance < SIGNIFICANCE) return Vector();

	Fresnel children = hit.entering ? fresnel(-direction, hit.normal, 1, 1.53) : fresnel(-direction, -hit.normal, 1.53, 1);
	
	real rand = random_real(random_generator);
	if (rand <= children.weight) {
	 	// Trace the reflective ray.
		Ray reflectiveRay(hit.position + (hit.entering ? hit.normal : -hit.normal) * EPSILON, children.reflect, TraceRes::ALL);
		Array<TraceRes> res2(scene->trace(reflectiveRay));
		if (res2.length() > 0) {
			Vector temp = res2[0].primitive->material->outgoingLight(scene, res2[0], -children.reflect, significance * children.weight);
			Vector colorized(temp.x * color.x, temp.y * color.y, temp.z * color.z);
			return colorized;
		}
	} else {
		// Trace the refractive ray.
		Ray refractiveRay(hit.position + (hit.entering ? -hit.normal : hit.normal) * EPSILON, children.refract, TraceRes::ALL);
		Array<TraceRes> res(scene->trace(refractiveRay));
		if (res.length() > 0) {
			Vector temp = res[0].primitive->material->outgoingLight(scene, res[0], -children.refract, significance * (1 - children.weight));
			Vector colorized(temp.x * pow(color.x, res[0].distance), temp.y * pow(color.y, res[0].distance), temp.z * pow(color.z, res[0].distance));
			return colorized;
		}
	}

	return Vector();
}

const char *Glass::name() const {
	return "Glass";
}

