#pragma once

#include "shape.h"
#include "material.h"

class Primitive : public Shape {
	public:
		Material *material;
	public:
		const char *name() const;
};

