#pragma once
#include "Config.h"
#include "Shape.h"
#include "TraceRes.h"
#include "Vector.h"
#include "Material.h"

class Diffuse : public Material {
	private:
		real diffuse;
		Vector color;
	public:
		Diffuse(real, const Vector&);

		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;

		virtual const char *name() const;
};

