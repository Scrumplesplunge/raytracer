#pragma once
#include "TraceRes.h"
#include "Ray.h"
#include "Vector.h"
#include "Array.h"

class Shape {
	public:
		virtual Array<TraceRes> trace(const Ray&) const = 0;
		virtual bool contains(const Vector&) const = 0;
		virtual const char *name() const;
};

