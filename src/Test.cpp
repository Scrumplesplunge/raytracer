#include "Config.h"
#include "Image.h"
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Array.h"
#include "TraceRes.h"
#include <cmath>

int main(int argc, char *args[]) {
	// Test image writing.
	Image canvas(8192, 6144);

	Sphere sphere(Vector(10, 0, 0), 3);

	for (unsigned int y = 0, maxY = canvas.getHeight(); y < maxY; y++) {
		for (unsigned int x = 0, maxX = canvas.getWidth(); x < maxX; x++) {
			Vector rayDirection(1, 2 * real(x) / real(maxX) - 1, (1 - 2 * real(y) / real(maxY)) * 0.75);
			Ray ray(Vector(0, 0, 0), rayDirection, TraceRes::DISTANCE | TraceRes::NORMAL);
			Array<TraceRes> res = sphere.trace(ray);
			
			unsigned char *pix = canvas(x, y);
			if (res.length() == 2) {
				pix[0] = (unsigned char)(127.9 * (res[1].normal.x + 1));
				pix[1] = (unsigned char)(127.9 * (res[1].normal.y + 1));
				pix[2] = (unsigned char)(127.9 * (res[1].normal.z + 1));
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

