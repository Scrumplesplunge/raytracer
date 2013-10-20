#pragma once
#include "Shape.h"
#include "Material.h"

class Primitive : public Shape {
	public:
		Material *material;
	public:
		const char *name() const;
};

