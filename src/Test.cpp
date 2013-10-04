#include "Config.h"
#include "Image.h"
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "CSGIntersection.h"
#include "CSGComplement.h"
#include "Array.h"
#include "TraceRes.h"
#include <cmath>
#include <iostream>

Sphere sphere(Vector(12, 0, 1), 3);
Sphere sphere2(Vector(8, 2, 0), 2);
Sphere sphere3(Vector(8, -1, 0), 2);
CSGComplement antisphere1(&sphere2);
CSGComplement antisphere2(&sphere3);
CSGIntersection both;

int main(int argc, char *args[]) {
	// Test image writing.
	Image canvas(1024, 768);

	both.add(&sphere);
	both.add(&antisphere1);
	both.add(&antisphere2);

	for (unsigned int y = 0, maxY = canvas.getHeight(); y < maxY; y++) {
		for (unsigned int x = 0, maxX = canvas.getWidth(); x < maxX; x++) {
			Vector rayDirection(1, 2 * real(x) / real(maxX) - 1, (1 - 2 * real(y) / real(maxY)) * 0.75);
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
	if (!canvas.save("test.bmp")) {
		canvas.printError();
		return 1;
	}

	return 0;
}

