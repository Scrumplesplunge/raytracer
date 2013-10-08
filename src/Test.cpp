#include "Config.h"
#include "Image.h"
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "CSGIntersection.h"
#include "CSGComplement.h"
#include "Array.h"
#include "TraceRes.h"
#include <cmath>
#include <iostream>

Sphere sphere(Vector(7, 0, 0), 3);
Sphere sphere2(Vector(3, 2, 0), 2);
Sphere sphere3(Vector(3, -1, 0), 2);
Sphere sphere4(Vector(7, 0, 0), 2.75);
Plane clip(Vector(5, 0, 0), Vector(-1, 0, 0));
CSGComplement antisphere1(&sphere2);
CSGComplement antisphere2(&sphere3);
CSGComplement antisphere3(&sphere4);
CSGIntersection both;

int main(int argc, char *args[]) {
	// Test image writing.
	Image canvas(1024, 1024);

	// Rotate sphere positions around Z-axis by theta.
	real theta = 0;

	both.add(&sphere);
	both.add(&clip);
	both.add(&antisphere1);
	both.add(&antisphere2);
	both.add(&antisphere3);

	for (unsigned int i = 0; i < 50; i++) {
		theta += 3.1416 / 50;
		real c = cos(theta), s = sin(theta);
		sphere2 = Sphere(Vector(7, 0, 0) + Vector(c * -10, s * -10, 0), 9);
		sphere3 = Sphere(Vector(7, 0, 0) + Vector(c * 10, s * 10, 0), 9);
		// End of rotation.

		for (unsigned int y = 0, maxY = canvas.getHeight(); y < maxY; y++) {
			for (unsigned int x = 0, maxX = canvas.getWidth(); x < maxX; x++) {
				Vector rayDirection(1, 2 * real(x) / real(maxX) - 1, (1 - 2 * real(y) / real(maxY)));
				Ray ray(Vector(0, 0, 0), rayDirection, TraceRes::DISTANCE | TraceRes::NORMAL);
				Array<TraceRes> res = both.trace(ray);
				
				unsigned char *pix = canvas(x, y);
				if (res.length() > 0) {
					pix[0] = (unsigned char)(127.9 * (res[0].normal.x + 1));
					pix[1] = (unsigned char)(127.9 * (res[0].normal.y + 1));
					pix[2] = (unsigned char)(127.9 * (res[0].normal.z + 1));
				} else {
					pix[0] = pix[1] = pix[2] = 0;
				}
			}
		}
	
		// Save the image.
		char filename[11] = {'t', 'e', 's', 't', '0', '0', '.', 'b', 'm', 'p', '\0'};
		filename[4] = '0' + i / 10;
		filename[5] = '0' + i % 10;
		std::cout << "Saving " << filename << ".. ";
		if (!canvas.save(filename)) {
			canvas.printError();
			return 1;
		}
		std::cout << "Done." << std::endl;
	}
	
	return 0;
}

