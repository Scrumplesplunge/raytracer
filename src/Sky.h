#pragma once
#include "Material.h"
#include "Image.h"

class Sky : public Material, public Image {
	public:
		Sky(const char*);

		virtual Vector outgoingLight(Shape*, const TraceRes&, const Vector&, real) const;

		virtual const char *name() const;
};

