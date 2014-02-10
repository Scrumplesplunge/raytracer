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
// Bit mask for float magic.
int hi = 0xffffffff, lo = 0x00000000;
__m128 mask = _mm_setr_ps(*(float*)&hi, *(float*)&hi, *(float*)&hi, *(float*)&lo);
__m128 half = _mm_setr_ps(0.5, 0.5, 0.5, 0);
__m128 threehalfs = _mm_setr_ps(1.5, 1.5, 1.5, 0);
#define shuf _MM_SHUFFLE(3, 0, 2, 1)

// Single precision vector operations.
Vector::Vector() : v(_mm_setzero_ps()) {}
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
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	#warning "I haven't implemented this yet :("
	#else
	// SPVO

	// Take the pairwise products.
	__m128 temp = _mm_mul_ps(v, v);

	// Take horizontal sums.
	temp = _mm_hadd_ps(temp, temp);
	temp = _mm_hadd_ps(temp, temp);

	// Half this value.
	__m128 x2 = _mm_mul_ps(temp, half);
	
	// Calculate inverse square.
	temp = _mm_rsqrt_ps(temp);

	// Improve this approximation.
	temp = _mm_mul_ps(temp, _mm_sub_ps(threehalfs, _mm_mul_ps(x2, _mm_mul_ps(temp, temp))));

	// Return new vector multiplied by this.
	return Vector(_mm_mul_ps(v, temp));
	#endif
	#else
	// SO
	return *this / length();
	#endif
}

real Vector::squareLength() const {
	#ifdef USE_SIMD
	#ifdef USE_DOUBLE
	// DPVO
	#warning "I haven't implemented this yet :("
	#else
	// SPVO

	// Take pairwise products.
	__m128 temp = _mm_mul_ps(v, v);

	// Sum horizontally.
	temp = _mm_hadd_ps(temp, temp);
	temp = _mm_hadd_ps(temp, temp);

	// Output.
	real out;
	_mm_store_ss(&out, temp);
	return out;
	#endif
	#else
	// SO
	return x * x + y * y + z * z;
	#endif
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
	return Vector(_mm_and_ps(_mm_add_ps(a.v, _mm_load1_ps(&k)), mask));
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
	return Vector(_mm_and_ps(_mm_add_ps(_mm_load1_ps(&k), a.v), mask));
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
	return Vector(_mm_and_ps(_mm_sub_ps(a.v, _mm_load1_ps(&k)), mask));
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
	return Vector(_mm_and_ps(_mm_sub_ps(_mm_load1_ps(&k), a.v), mask));
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
	__m128 A = a.v, B = b.v;
	__m128 temp = _mm_sub_ps(
		_mm_mul_ps(
			A,
			_mm_shuffle_ps(B, B, shuf)
		),
		_mm_mul_ps(
			_mm_shuffle_ps(A, A, shuf),
			B
		)
	);
	return Vector(_mm_shuffle_ps(temp, temp, shuf));
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

