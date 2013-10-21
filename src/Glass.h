#pragma once
#include "Config.h"
#include "Shape.h"
#include "TraceRes.h"
#include "Vector.h"
#include "Material.h"

class Glass : public Material {
	private:
		Vector color;
	public:
		Glass(const Vector&);

		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;

		virtual const char *name() const;
};

