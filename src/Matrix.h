#pragma once
#include "Config.h"
#include "Vector.h"

class Matrix {
	public:
		Vector a, b, c, d;
	public:
		Matrix();
		Matrix(const Vector&, const Vector&, const Vector&, const Vector&);
		
		Matrix translate(const Vector&) const;
		Matrix rotateX(real) const;
		Matrix rotateY(real) const;
		Matrix rotateZ(real) const;

		Vector operator()(const Vector&) const;
};

Vector operator*(const Matrix&, const Vector&);

