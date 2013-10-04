#pragma once
#include "Vector.h"

class Ray {
	public:
		Vector start, direction;
		unsigned int mask;

		Ray(const Vector&, const Vector&, unsigned int);
};

