#include "Test.h"
#include "../src/Vector.h"

class VectorTest : public Test {
	private:
		int eq(std::string name, real a, real b) {
			std::cout << "Testing " << note << name << norm << " : ";
			if (equal(a, b)) {
				std::cout << good << "Value is " << b << norm << std::endl;
				return 0;
			} else {
				std::cout << good << "Value " << a << " is not equal to target value " << b << bad << std::endl;
				return 1;
			}
		}

		int eq(std::string name, const Vector& v, real a, real b, real c) {
			std::cout << "Testing " << note << name << norm << " : ";
			if (equal(v.x, a) && equal(v.y, b) && equal(v.z, c)) {
				std::cout << good << "Vector equals [" << a << ", " << b << ", " << c << "]" << norm << std::endl;
				return 0;
			} else {
				std::cout << bad << "Vector [" << v.x << ", " << v.y << ", " << v.z << "] is not equal to target vector [" << a << ", " << b << ", " << c << "]" << norm << std::endl;
				return 1;
			}
		}

		int eq(std::string name, const Vector& a, const Vector& b) {
			return eq(name, a, b.x, b.y, b.z);
		}
	public:
		int perform() {
			std::cout << "Performing " << note << "Correctness Tests" << norm << ":" << std::endl;
			int errors = 0;
			const Vector va(1, 2, 3);
			const Vector vb(3, 2, 1);

			// Check that the values are stored correctly.
			errors += eq("A", va, 1, 2, 3);
			errors += eq("B", vb, 3, 2, 1);

			// Addition tests.
			const Vector vc = va + vb;
			errors += eq("C = A + B", vc, 4, 4, 4);
			const Vector vd = 1 + va;
			errors += eq("D = 1 + A", vd, 2, 3, 4);
			const Vector ve = vb + 1;
			errors += eq("E = B + 1", ve, 4, 3, 2);

			// Negation and subtraction.
			const Vector vf = va - vb;
			errors += eq("F = A - B", vf, -2, 0, 2);
			const Vector vg = 1 - va;
			errors += eq("G = 1 - A", vg, 0, -1, -2);
			const Vector vh = vb - 1;
			errors += eq("H = B - 1", vh, 2, 1, 0);
			const Vector vi = -va;
			errors += eq("I = -A", vg, -1, -2, -3);

			// Multiplication tests.
			const Vector vj = va * 2;
			errors += eq("J = A * 2", vh, 2, 4, 6);
			const Vector vk = 2 * vb;
			errors += eq("K = 2 * B", vi, 6, 4, 2);

			// Division test.
			const Vector vl = va / 2;
			errors += eq("L = A / 2", vj, 0.5, 1, 1.5);
			
			// Dot product.
			eq("dot(A, B)", dot(va, vb), 10);

			// Cross product.
			const Vector vm = cross(va, vb);
			eq("M = cross(A, B)", vm, -4, 8, -4);

			// Value of A and B at the end.
			eq("A == A0", va, 1, 2, 3);
			eq("B == B0", vb, 3, 2, 1);

			// End of correctness tests.
			std::cout << note << "Correctness Tests" << norm << " complete: " << (errors == 0 ? good : bad) << errors << " errors" << norm << ".\n" << std::endl;

			// Performance tests.
			std::cout << "Performing " << note << "Performance Tests" << norm << ":" << std::endl;
			float start, end;

			std::cout << "Testing " << note << "dot product" << norm << " : ";
			real k;
			start = getTime();
			for (unsigned int i = 0; i < 1000000000; i++) {
				k += dot(va, vb);
			}
			end = getTime();
			std::cout << "Completed in " << note << (end - start) << " seconds" << norm << "." << std::endl;

			return errors;
		}

		std::string name() {
			return "Vector Correctness and Performance";
		}
};

VectorTest _vectorTest = VectorTest();
Test *vectorTest = &_vectorTest;

