#pragma once
#include "Config.h"

class Vector {
	public:
		real x, y, z;
		
		Vector();
		Vector(real, real, real);

		Vector setX(real) const;
		Vector setY(real) const;
		Vector setZ(real) const;

		Vector normalized() const;
		real squareLength() const;
		real length() const;
};

Vector operator+(const Vector&, const Vector&);
Vector operator+(const Vector&, real);
Vector operator+(real, const Vector&);

Vector operator-(const Vector&, const Vector&);
Vector operator-(const Vector&, real);
Vector operator-(real, const Vector&);
Vector operator-(const Vector&);

Vector operator*(const Vector&, real);
Vector operator*(real, const Vector&);

Vector operator/(const Vector&, real);

Vector cross(const Vector&, const Vector&);

real dot(const Vector&, const Vector&);

