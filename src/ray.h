#pragma once

#include "vector.h"

class Ray {
	public:
		Vector start, direction;
		unsigned int mask;

		Ray(const Vector&, const Vector&, unsigned int);
};

