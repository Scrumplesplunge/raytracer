#include "Matrix.h"
#include <cmath>

Matrix::Matrix() : a(1, 0, 0), b(0, 1, 0), c(0, 0, 1), d(0, 0, 0) {}

Matrix::Matrix(const Vector& p, const Vector& q, const Vector& r, const Vector& s) : a(p), b(q), c(r), d(s) {}

Matrix Matrix::translate(const Vector& t) const {
	return Matrix(a, b, c, d + t);
}

Matrix Matrix::rotateX(real ang) const {
	real ca = cos(ang), sa = sin(ang);
	Vector q(0, ca, -sa), r(0, sa, ca);
	return Matrix(
		Vector(a.x, q.dot(a), r.dot(a)),
		Vector(b.x, q.dot(b), r.dot(b)),
		Vector(c.x, q.dot(c), r.dot(c)),
		Vector(d.x, q.dot(d), r.dot(d))
	);
}

Matrix Matrix::rotateY(real ang) const {
	real ca = cos(ang), sa = sin(ang);
	Vector p(ca, 0, -sa), r(sa, 0, ca);
	return Matrix(
		Vector(p.dot(a), a.y, r.dot(a)),
		Vector(p.dot(b), b.y, r.dot(b)),
		Vector(p.dot(c), c.y, r.dot(c)),
		Vector(p.dot(d), d.y, r.dot(d))
	);
}

Matrix Matrix::rotateZ(real ang) const {
	real ca = cos(ang), sa = sin(ang);
	Vector p(ca, -sa, 0), q(sa, ca, 0);
	return Matrix(
		Vector(p.dot(a), q.dot(a), a.z),
		Vector(p.dot(b), q.dot(b), b.z),
		Vector(p.dot(c), q.dot(c), c.z),
		Vector(p.dot(d), q.dot(d), d.z)
	);
}

Vector operator*(const Matrix& m, const Vector& v) {
	return Vector(
		m.a.x * v.x + m.b.x * v.y + m.c.x * v.z + m.d.x,
		m.a.y * v.x + m.b.y * v.y + m.c.y * v.z + m.d.y,
		m.a.z * v.x + m.b.z * v.y + m.c.z * v.z + m.d.z
	);
}

