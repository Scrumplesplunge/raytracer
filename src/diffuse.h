#pragma once

#include "config.h"
#include "shape.h"
#include "trace_res.h"
#include "vector.h"
#include "material.h"

class Diffuse : public Material {
	private:
		real diffuse;
		Vector color;
	public:
		Diffuse(real, const Vector&);

		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;

		virtual const char *name() const;
};

