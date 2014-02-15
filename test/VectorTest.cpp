#include "Test.h"
#include "../src/Vector.h"

class VectorTest : public Test {
	private:
		int eq(std::string name, real a, real b) {
			test(name);
			if (equal(a, b)) {
				std::cout << good << "Value is " << b << norm << std::endl;
				return 0;
			} else {
				std::cout << good << "Value " << a << " is not equal to target value " << b << bad << std::endl;
				return 1;
			}
		}

		int eq(std::string name, const Vector& v, real a, real b, real c) {
			test(name);
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
			std::cout << "Performing " << note << "Correctness Tests" << norm << " :" << std::endl;
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
			errors += eq("I = -A", vi, -1, -2, -3);

			// Multiplication tests.
			const Vector vj = va * 2;
			errors += eq("J = A * 2", vj, 2, 4, 6);
			const Vector vk = 2 * vb;
			errors += eq("K = 2 * B", vk, 6, 4, 2);

			// Division test.
			const Vector vl = va / 2;
			errors += eq("L = A / 2", vl, 0.5, 1, 1.5);

			// SquareLength.
			errors += eq("A.squareLength()", va.squareLength(), 14);

			// Length.
			errors += eq("A.length()", va.length(), 3.7416573867739413);

			// Normalized version.
			errors += eq("A.normalized()", va.normalized(), 0.2672612419124244, 0.5345224838248488, 0.8017837257372732);
			
			// Dot product.
			errors += eq("dot(A, B)", dot(va, vb), 10);

			// Cross product.
			const Vector vm = cross(va, vb);
			errors += eq("M = cross(A, B)", vm, -4, 8, -4);

			// Value of A and B at the end.
			errors += eq("A == A0", va, 1, 2, 3);
			errors += eq("B == B0", vb, 3, 2, 1);

			// End of correctness tests.
			std::cout << note << "Correctness Tests" << norm << " complete : " << (errors == 0 ? good : bad) << errors << " errors" << norm << ".\n" << std::endl;

			// Performance tests.
			std::cout << "Performing " << note << "Performance Tests" << norm << " :" << std::endl;
			float start, end;

			unsigned int reps = 1e9;

			test("Dot Product");
			real k;
			start = getTime();
			for (unsigned int i = 0; i < reps; i++) {
				k += dot(va, vb);
			}
			end = getTime();
			perf(end - start, reps);

			test("Cross Product");
			Vector vn = va;
			start = getTime();
			for (unsigned int i = 0; i < reps; i++) {
				vn = cross(vn, vb);
			}
			end = getTime();
			perf(end - start, reps);

			test("Normalization");
			Vector vo;
			start = getTime();
			for (unsigned int i = 0; i < reps; i++) {
				vo = va.normalized();
			}
			end = getTime();
			perf(end - start, reps);

			// End of performance tests.
			std::cout << note << "Performance Tests" << norm << " complete." << std::endl;

			return errors;
		}

		std::string name() const {
			return Vector::type() + std::string(" Correctness and Performance Tests");
		}
};

VectorTest _vectorTest = VectorTest();
Test *vectorTest = &_vectorTest;

