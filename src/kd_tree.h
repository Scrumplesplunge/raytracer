#pragma once

#include "vector.h"
#include "shape.h"

class KDTree : public Shape {
	private:
		real split;
		char splitDirection;
		Array<Shape*> a, b;
	public:
		
};

