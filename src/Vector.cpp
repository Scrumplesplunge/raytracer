#include "Vector.h"
#include <cmath>

Vector::Vector() : x(0), y(0), z(0) {}

Vector::Vector(real a, real b, real c) : x(a), y(b), z(c) {}

Vector Vector::setX(real a) const {
	return Vector(a, y, z);
}

Vector Vector::setY(real b) const {
	return Vector(x, b, z);
}

Vector Vector::setZ(real c) const {
	return Vector(x, y, c);
}

Vector Vector::normalized() const {
	return *this / length();
}

real Vector::squareLength() const {
	return x * x + y * y + z * z;
}

real Vector::length() const {
	return sqrt(squareLength());
}

Vector operator+(const Vector& a, const Vector& b) {
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector operator+(const Vector& a, real k) {
	return Vector(a.x + k, a.y + k, a.z + k);
}

Vector operator+(real k, const Vector& a) {
	return Vector(k + a.x, k + a.y, k + a.z);
}

Vector operator-(const Vector& a, const Vector& b) {
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector operator-(const Vector& a, real k) {
	return Vector(a.x - k, a.y - k, a.z - k);
}

Vector operator*(const Vector& a, real k) {
	return Vector(a.x * k, a.y * k, a.z * k);
}

Vector operator*(real k, const Vector& a) {
	return Vector(k * a.x, k * a.y, k * a.z);
}

Vector operator/(const Vector& a, real k) {
	real inv = 1 / k;
	return a * inv;
}

Vector cross(const Vector& a, const Vector& b) {
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

real dot(const Vector& a, const Vector& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

