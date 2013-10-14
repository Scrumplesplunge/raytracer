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

		virtual real minX() const = 0;
		virtual real minY() const = 0;
		virtual real minZ() const = 0;

		virtual real maxX() const = 0;
		virtual real maxY() const = 0;
		virtual real maxZ() const = 0;
};

