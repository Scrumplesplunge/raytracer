#include "Material.h"

Vector Material::refract(const Vector& vec, const Vector& norm, real refractiveIndex1, real refractiveIndex2) {
	// This is an approximation.  We can sub-in a proper solution at a later date.
	real mul = refractiveIndex1 - refractiveIndex2;
	return vec - norm * mul;
}

Vector Material::reflect(const Vector& vec, const Vector& norm) {
	real mul = 2 * dot(vec, norm);
	return vec - norm * mul;
}

const char *Material::name() const {
	return "Undefined Material";
};

