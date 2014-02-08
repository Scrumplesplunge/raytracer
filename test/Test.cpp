#include "../src/Vector.h"
#include <iostream>
#include <sstream>
#include <string>

std::string col(int code) {
	std::stringstream buf;
	buf << char(0x1B) << '[' << code << 'm';
	return buf.str();
}

#define good col(32)
#define bad col(31)
#define norm col(37)

bool fe(real a, real b) {
	return a - EPSILON < b && b < a + EPSILON;
}

int eq(const char *name, real a, real b) {
	std::cout << "Testing " << name << " : ";
	if (fe(a, b)) {
		std::cout << col(32) << "Value is " << b << norm << std::endl;
		return 0;
	} else {
		std::cout << bad << "Value " << a << " is not equal to target " << b << norm << std::endl;
		return 1;
	}
}

int eq(const char *name, const Vector& v, real a, real b, real c) {
	std::cout << "Testing " << name << " : ";
	if (fe(v.x, a) && fe(v.y, b) && fe(v.z, c)) {
		std::cout << good << "Vector equals [" << a << ", " << b << ", " << c << "]" << norm << std::endl;
		return 0;
	} else {
		std::cout << bad << "Vector [" << v.x << ", " << v.y << ", " << v.z << "] not equal to target [" << a << ", " << b << ", " << c << "]" << norm << std::endl;
		return 1;
	}
}

int eq(const char *name, const Vector& a, const Vector& b) {
	return eq(name, a, b.x, b.y, b.z);
}

int main() {
	int errors = 0;

	Vector a(1.0, 2.0, 3.0);
	Vector b(3.0, 2.0, 1);

	// Check that they equal what they were told to equal!
	errors += eq("A", a, 1.0, 2.0, 3.0);
	errors += eq("B", b, 3.0, 2.0, 1.0);

	// Addition tests.
	Vector c = a + b;
	errors += eq("C = A + B", c, 4.0, 4.0, 4.0);
	Vector d = 1 + a;
	errors += eq("D = 1 + A", d, 2.0, 3.0, 4.0);
	Vector e = b + 1;
	errors += eq("E = B + 1", e, 4.0, 3.0, 2.0);

	// Subtraction tests.
	Vector f = a - b;
	errors += eq("F = A - B", f, -2.0, 0.0, 2.0);

	// Negation tests.
	Vector g = -a;
	errors += eq("G = -A", g, -1.0, -2.0, -3.0);
	
	// Multiplication tests.
	Vector h = a * 2;
	errors += eq("H = A * 2", h, 2.0, 4.0, 6.0);
	Vector i = 2 * b;
	errors += eq("I = 2 * B", i, 6.0, 4.0, 2.0);

	// Dot product test.
	eq("dot(A, B)", dot(a, b), 10.0);

	// Cross product test.
	Vector j = cross(a, b);
	eq("cross(A, B)", j, -4.0, 8.0, -4.0);

	std::cout << std::endl;
	std::cout << "Tests finished: " << (errors == 0 ? good : bad) << errors << norm << " errors." << std::endl;
	return 0;
}

