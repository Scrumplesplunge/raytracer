#pragma once

//#define USE_SIMD
#define USE_DOUBLE

#ifdef USE_DOUBLE
typedef double real;
const real EPSILON = 0.00001;
#else
typedef float real;
const real EPSILON = 0.001;
#endif

const real SIGNIFICANCE = 1e-3;
const real HALFPI = 1.5707963267948966;
const real PI = 3.141592653589793238463;
const real TWOPI = 6.283185307179586;
const real SQRT2 = 1.4142135623730951;
const real SQRT2_2 = 0.7071067811865476;
const real INFINITY = 1e25;

