#pragma once
#include "Vector.h"
#include "Shape.h"
#include "TraceRes.h"
#include "Material.h"

class Light : public Material {
	private:
		Vector color;
	public:
		Light(const Vector&);

		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;

		virtual const char *name() const;
};

