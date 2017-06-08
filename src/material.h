#pragma once

#include "config.h"
#include "vector.h"
#include "trace_res.h"
#include "shape.h"

class Material {
	protected:
		struct Fresnel {
			real weight;
			Vector reflect, refract;
		};

		static Vector refract(const Vector&, const Vector&, real, real);
		static Vector reflect(const Vector&, const Vector&);
		static Fresnel fresnel(const Vector&, const Vector&, real, real);
	public:
		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const = 0;

		virtual const char *name() const;
};
