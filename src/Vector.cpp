#include "Vector.h"
#include <cmath>
#include "Random.h"

std::normal_distribution<real> vector_rand(0, 1);

#ifdef USE_SIMD
#ifdef USE_DOUBLE
// Double precision plus vector operations.
Vector::Vector() : v1(), v2() {}
Vector::Vector(real a, real b, real c) : v1(_mm_setr_pd(a, b)), v2(_mm_setr_pd(c, 0)) {}
Vector::Vector(__m128 a, __m128 b) : v1(a), v2(b) {}
#else
// Single precision vector operations.
Vector::Vector() : v() {}
Vector::Vector(real a, real b, real c) : v(_mm_setr_ps(a, b, c, 0)) {}
Vector::Vector(__m128 a) : v(a) {}
#endif
#else
// No vector ops.
Vector::Vector() : x(0), y(0), z(0) {}
Vector::Vector(real a, real b, real c) : x(a), y(b), z(c) {}
#endif

Vector Vector::setX(real a) const {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	return Vector(_mm_setr_pd(a, y), v2);
	#else
	// SPVO
	return Vector(_mm_setr_ps(a, y, z, 0));
	#endif
	#else
	// SO
	return Vector(a, y, z);
	#endif
}

Vector Vector::setY(real b) const {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	return Vector(_mm_setr_pd(x, b), v2);
	#else
	// SPVO
	return Vector(_mm_setr_ps(x, b, z, 0));
	#endif
	#else
	// SO
	return Vector(x, b, z);
	#endif
}

Vector Vector::setZ(real c) const {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	return Vector(v2, _mm_setr_pd(c, 0));
	#else
	// SPVO
	return Vector(_mm_setr_ps(x, y, c, 0));
	#endif
	#else
	// SO
	return Vector(x, y, c);
	#endif
}

Vector Vector::normalized() const {
	return *this / length();
}

real Vector::squareLength() const {
	return x * x + y * y + z * z;
}

real Vector::length() const {
	return sqrt(squareLength());
}

Vector Vector::random() {
	return Vector(vector_rand(random_generator), vector_rand(random_generator), vector_rand(random_generator));
}

const char *Vector::type() {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	return "Double-Precision SSE Vector";
	#else
	// SPVO
	return "Single-Precision SSE Vector";
	#endif
	#else
	#ifdef USE_DOUBLE
	// DPSO
	return "Double-Precision Serial Vector";
	#else
	// SPSO
	return "Single-Precision Serial Vector";
	#endif
	#endif
}

Vector operator+(const Vector& a, const Vector& b) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	return Vector(_mm_add_pd(a.v1, b.v1), _mm_add_pd(a.v2, b.v2));
	#else
	// SPVO
	return Vector(_mm_add_ps(a.v, b.v));
	#endif
	#else
	// SO
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
	#endif
}

Vector operator+(const Vector& a, real k) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_add_pd(a.v1, temp), _mm_add_pd(a.v2, temp));
	#else
	// SPVO
	return Vector(_mm_add_ps(a.v, _mm_load1_ps(&k)));
	#endif
	#else
	// SO
	return Vector(a.x + k, a.y + k, a.z + k);
	#endif
}

Vector operator+(real k, const Vector& a) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_add_pd(temp, a.v1), _mm_add_pd(temp, a.v2));
	#else
	// SPVO
	return Vector(_mm_add_ps(_mm_load1_ps(&k), a.v));
	#endif
	#else
	// SO
	return Vector(k + a.x, k + a.y, k + a.z);
	#endif
}

Vector operator-(const Vector& a, const Vector& b) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	return Vector(_mm_sub_pd(a.v1, b.v1), _mm_sub_pd(a.v2, b.v2));
	#else
	// SPVO
	return Vector(_mm_sub_ps(a.v, b.v));
	#endif
	#else
	// SO
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
	#endif
}

Vector operator-(const Vector& a, real k) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_sub_pd(a.v1, temp), _mm_sub_pd(a.v2, temp));
	#else
	// SPVO
	return Vector(_mm_sub_ps(a.v, _mm_load1_ps(&k)));
	#endif
	#else
	// SO
	return Vector(a.x - k, a.y - k, a.z - k);
	#endif
}

Vector operator-(real k, const Vector& a) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_sub_pd(temp, a.v1), _mm_sub_pd(temp, a.v2));
	#else
	// SPVO
	return Vector(_mm_sub_ps(_mm_load1_ps(&k), a.v));
	#endif
	#else
	// SO
	return Vector(k - a.x, k - a.y, k - a.z);
	#endif
}

Vector operator-(const Vector& a) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_setzero_pd();
	return Vector(_mm_sub_pd(temp, a.v1), _mm_sub_pd(temp, a.v2));
	#else
	// SPVO
	__m128 temp = _mm_setzero_ps();
	return Vector(_mm_sub_ps(temp, a.v));
	#endif
	#else
	// SO
	return Vector(-a.x, -a.y, -a.z);
	#endif
}

Vector operator*(const Vector& a, real k) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_mul_pd(a.v1, temp), _mm_mul_pd(a.v2, temp));
	#else
	// SPVO
	return Vector(_mm_mul_ps(a.v, _mm_load1_ps(&k)));
	#endif
	#else
	// SO
	return Vector(a.x * k, a.y * k, a.z * k);
	#endif
}

Vector operator*(real k, const Vector& a) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_mul_pd(temp, a.v1), _mm_mul_pd(temp, a.v2));
	#else
	// SPVO
	return Vector(_mm_mul_ps(_mm_load1_ps(&k), a.v));
	#endif
	#else
	// SO
	return Vector(k * a.x, k * a.y, k * a.z);
	#endif
}

Vector operator/(const Vector& a, real k) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	__m128 temp = _mm_load1_pd(&k);
	return Vector(_mm_div_pd(a.v1, temp), _mm_div_pd(a.v2, temp));
	#else
	// SPVO
	return Vector(_mm_div_ps(a.v, _mm_load1_ps(&k)));
	#endif
	#else
	// SO
	real inv = 1 / k;
	return Vector(a.x * inv, a.y * inv, a.z * inv);
	#endif
}

Vector cross(const Vector& a, const Vector& b) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	#warning "I haven't implemented this yet :("
	#else
	// SPVO
	__m128 temp = _mm_sub_ps(
		_mm_mul_ps(
			a.v,
			_mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(0, 0, 2, 1))
		),
		_mm_mul_ps(
			_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(0, 0, 2, 1)),
			b.v
		)
	);
	return Vector(_mm_shuffle_ps(temp, temp, _MM_SHUFFLE(3, 0, 2, 1)));
	#endif
	#else
	// SO
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	#endif
}

real dot(const Vector& a, const Vector& b) {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	#warning "I haven't implemented this yet :("
	#else
	// SPVO

	// Take the pairwise products.
	__m128 temp = _mm_mul_ps(a.v, b.v);

	// Normalize to only (x, y, z, 0)
	temp = _mm_shuffle_ps(temp, _mm_shuffle_ps(temp, _mm_setzero_ps(), _MM_SHUFFLE(0, 0, 0, 2)), _MM_SHUFFLE(3, 0, 1, 0));

	// Take horizontal sums.
	temp = _mm_hadd_ps(temp, temp);
	temp = _mm_hadd_ps(temp, temp);
	real out;
	_mm_store_ss(&out, temp);
	return out;
	#endif
	#else
	// SO
	return a.x * b.x + a.y * b.y + a.z * b.z;
	#endif
}

