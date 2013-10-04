#pragma once
#include "Shape.h"

class Primitive : public Shape {
	public:
		const char *name() const;
};

