#pragma once
#include "Config.h"
#include "Shape.h"
#include "TraceRes.h"
#include "Vector.h"
#include "Material.h"

class Glass : public Material {
	public:
		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;
		virtual real refractiveIndex() const;

		virtual const char *name() const;
};

