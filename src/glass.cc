#include "glass.h"
#include "ray.h"
#include "primitive.h"
#include "random.h"

#include <cmath>
#include <algorithm>

std::uniform_real_distribution<real> random_real(0, 1);

Glass::Glass(const Vector& col) : color(col) {}

Vector Glass::outgoingLight(Shape *scene, const TraceRes& hit, const Vector& direction, real significance) const {
	// Be really lazy if the significance is low enough.
	if (significance < SIGNIFICANCE) return Vector();

	Vector norm = hit.entering ? hit.normal : -hit.normal;

	Fresnel children = fresnel(-direction, norm, hit.entering ? 1 : 1.53, hit.entering ? 1.53 : 1);
	
	real rand = random_real(random_generator);
	if (rand <= children.weight) {
	 	// Trace the reflective ray.
		Ray reflectiveRay(hit.position + norm * EPSILON, children.reflect, TraceRes::ALL);
		Array<TraceRes> res2(scene->trace(reflectiveRay));
		if (res2.length() > 0) {
			real mul = children.weight;
			Vector temp = res2[0].primitive->material->outgoingLight(scene, res2[0], -children.reflect, significance * mul);
			Vector colorized(temp.x * color.x, temp.y * color.y, temp.z * color.z);
			return colorized;
		}
	} else {
		// Trace the refractive ray.
		Ray refractiveRay(hit.position - norm * EPSILON, children.refract, TraceRes::ALL);
		Array<TraceRes> res(scene->trace(refractiveRay));
		if (res.length() > 0) {
			real mul = (1 - children.weight);
			Vector temp = res[0].primitive->material->outgoingLight(scene, res[0], -children.refract, significance * mul);
			if (hit.entering) {
				return Vector(temp.x * pow(color.x, res[0].distance), temp.y * pow(color.y, res[0].distance), temp.z * pow(color.z, res[0].distance));
			} else {
				return temp;
			}
		}
	}

	return Vector();
}

const char *Glass::name() const {
	return "Glass";
}

