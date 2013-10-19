#pragma once
#include "Material.h"

class Glass : public Material {
	public:
		virtual Vector outgoingLight(const TraceRes&, const Vector&) const;
		virtual real refractiveIndex() const;

		virtual const char *name() const;
};

