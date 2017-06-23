#include "material.h"

Vector Material::refract(Vector vector, Vector normal,
                         real refractive_index_1, real refractive_index_2) {
  // TODO: This is an approximation. Sub-in a proper solution.
  Vector direction =
      vector + normal * (refractive_index_1 - refractive_index_2);
  return direction.normalized();
}

Vector Material::reflect(Vector vector, Vector normal) {
  return vector - 2 * dot(normal, vector) * normal;
}

Material::Fresnel Material::fresnel(Vector vector, Vector normal,
                                    real refractive_index_1,
                                    real refractive_index_2) {
  Fresnel out;
  out.refract = refract(vector, normal, refractive_index_1, refractive_index_2);
  out.reflect = reflect(vector, normal);

  real cosI = -dot(normal, vector);
  real cosT = -dot(normal, out.refract);
  real sPolarReflect = (refractive_index_1 * cosI - refractive_index_2 * cosT) /
                       (refractive_index_1 * cosI + refractive_index_2 * cosT);
  sPolarReflect *= sPolarReflect;
  real pPolarReflect = (refractive_index_1 * cosT - refractive_index_2 * cosI) /
                       (refractive_index_1 * cosT + refractive_index_2 * cosI);
  pPolarReflect *= pPolarReflect;
  out.fraction_reflected =
      std::min(real{1}, (sPolarReflect + pPolarReflect) / 2);
  return out;
}
