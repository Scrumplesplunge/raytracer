#pragma once
#include "Vector.h"
#include "Shape.h"

class KDTree : public Shape {
	private:
		real split;
		char splitDirection;
		Array<Shape*> a, b;
	public:
		
};

