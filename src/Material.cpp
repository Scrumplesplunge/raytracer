#include "Material.h"

Vector Material::refract(const Vector& vec, const Vector& norm, real refractiveIndex1, real refractiveIndex2) {
	// This is an approximation.  We can sub-in a proper solution at a later date.
	real mul = refractiveIndex1 - refractiveIndex2;
	return vec - norm * mul;
}

Vector Material::reflect(const Vector& vec, const Vector& norm) {
	real mul = 2 * dot(vec, norm);
	return 2 * vec - norm * mul;
}

Material::Fresnel Material::fresnel(const Vector& vec, const Vector& norm, real refractiveIndex1, real refractiveIndex2) {
	Fresnel out;
	out.refract = refract(vec, norm, refractiveIndex1, refractiveIndex2).normalized();
	out.reflect = reflect(vec, norm);

	real cosI = -dot(norm, vec);
	real cosT = -dot(norm, out.refract);
	real sPolarReflect = (refractiveIndex1 * cosI - refractiveIndex2 * cosT) / (refractiveIndex1 * cosI + refractiveIndex2 * cosT);
	sPolarReflect *= sPolarReflect;
	real pPolarReflect = (refractiveIndex1 * cosT - refractiveIndex2 * cosI) / (refractiveIndex1 * cosT + refractiveIndex2 * cosI);
	sPolarReflect *= sPolarReflect;
	out.weight = (sPolarReflect + pPolarReflect) / 2;
	if (out.weight > 1) out.weight = 1;

	return out;
}

const char *Material::name() const {
	return "Undefined Material";
};

