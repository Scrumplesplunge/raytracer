#pragma once
#include "Config.h"
#include "Vector.h"
#include "TraceRes.h"

class Material {
	protected:
		static Vector refract(const Vector&, const Vector&, real, real);
		static Vector reflect(const Vector&, const Vector&);
	public:
		virtual Vector outgoingLight(const TraceRes&, const Vector&) const = 0;
		virtual real refractiveIndex() const = 0;

		virtual const char *name() const;
};

