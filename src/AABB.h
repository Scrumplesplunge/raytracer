#pragma once
#include "Config.h"
#include "Vector.h"
#include "Ray.h"

class AABB {
	private:
		Vector min, max;
	public:
		AABB(const Vector&, const Vector&);

		bool obstructs(const Ray&) const;
};

