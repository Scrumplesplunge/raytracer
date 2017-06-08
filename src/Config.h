#pragma once

#include <math.h>

//#define USE_SIMD
//#define USE_DOUBLE

#ifdef USE_DOUBLE
typedef double real;
const real EPSILON = 0.00001;
#else
typedef float real;
const real EPSILON = 0.001;
#endif

const real SIGNIFICANCE = 0.01;
const real PI = 3.141592653589793238463;
const real HALFPI = 0.5 * PI;
const real TWOPI = 2.0 * PI;
const real SQRT2 = sqrt(2.0);
const real SQRT2_2 = SQRT2 / 2.0;
