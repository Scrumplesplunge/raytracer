#pragma once

#include "vector.h"
#include "shape.h"
#include "trace_res.h"
#include "material.h"

class Light : public Material {
	private:
		Vector color;
	public:
		Light(const Vector&);

		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;

		virtual const char *name() const;
};

