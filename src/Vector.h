#pragma once
#include "Config.h"

#ifdef USE_SIMD
#include <xmmintrin.h>
#endif

#define align16 __attribute__((aligned (16)))

class Vector {
	public:
		#ifdef USE_SIMD
		#ifdef USE_DOUBLE
		// Double precision parallel operations.
		union align16 {
			struct {
				__m128 v1, v2;
			};
			struct {
				double x;
				double y;
				double z;
				double _;
			};
		};
		#else
		// Single precision parallel operations.
		union align16 {
			struct {
				__m128 v;
			};
			struct {
				float x;
				float y;
				float z;
				float _;
			};
		};
		#endif
		#else
		// Single precision sequential operations.
		real x, y, z;
		#endif
		
		Vector();
		Vector(real, real, real);

		#ifdef USE_SIMD
		#ifdef USE_DOUBLE
		// Double precision parallel vectors require two registers.
		Vector(__m128, __m128);
		#else
		// Single precision require one.
		Vector(__m128);
		#endif
		#endif

		Vector setX(real) const;
		Vector setY(real) const;
		Vector setZ(real) const;

		Vector normalized() const;
		real squareLength() const;
		real length() const;

		static Vector random();
		static const char *type();
};

Vector operator+(const Vector&, const Vector&);
Vector operator+(const Vector&, real);
Vector operator+(real, const Vector&);

Vector operator-(const Vector&, const Vector&);
Vector operator-(const Vector&, real);
Vector operator-(real, const Vector&);
Vector operator-(const Vector&);

Vector operator*(const Vector&, real);
Vector operator*(real, const Vector&);

Vector operator/(const Vector&, real);

Vector cross(const Vector&, const Vector&);

real dot(const Vector&, const Vector&);

