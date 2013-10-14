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

		virtual real minX() const;
		virtual real minY() const;
		virtual real minZ() const;

		virtual real maxX() const;
		virtual real maxY() const;
		virtual real maxZ() const;
};

