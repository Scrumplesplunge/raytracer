#include "Diffuse.h"
#include "Config.h"
#include "Primitive.h"
#include "Random.h"
#include <cmath>

std::uniform_real_distribution<real> random_real_diffuse(0, 1);

Diffuse::Diffuse(real diff, const Vector& col) : diffuse(diff), color(col) {}

Vector Diffuse::outgoingLight(Shape *scene, const TraceRes& hit, const Vector& direction, real significance) const {
	// Be lazy if significance is low enough.
	if (significance < SIGNIFICANCE) return Vector();
	
	// Generate a random ray.
	Vector vec;
	if (random_real_diffuse(random_generator) > diffuse) {
		vec = reflect(-direction, hit.normal);
	} else {
		vec = Vector::random();
	}

	real mul = dot(vec, hit.normal);
	if (mul < 0) {
		vec = -vec;
		mul = -mul;
	}
	
	Ray randomRay(hit.position + hit.normal * EPSILON, vec, TraceRes::ALL);
	Array<TraceRes> res(scene->trace(randomRay));
	Vector temp;
	if (res.length() > 0) temp = mul * res[0].primitive->material->outgoingLight(scene, res[0], -vec, significance * mul);

	return Vector(temp.x * color.x, temp.y * color.y, temp.z * color.z);
}

const char *Diffuse::name() const {
	return "Diffuse";
}

