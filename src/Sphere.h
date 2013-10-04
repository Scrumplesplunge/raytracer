#pragma once
#include "Config.h"
#include "Primitive.h"
#include "Vector.h"

class Sphere : public Primitive {
	private:
		Vector position;
		real radius, squareRadius;
	public:
		Sphere(const Vector&, real);

		virtual Array<TraceRes> trace(const Ray&) const;
		virtual bool contains(const Vector&) const;
		virtual const char *name() const;
};

