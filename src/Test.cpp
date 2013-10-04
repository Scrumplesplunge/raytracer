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
	Image canvas(640, 480);

	Sphere sphere(Vector(10, 0, 0), 3);

	for (unsigned int y = 0, maxY = canvas.getHeight(); y < maxY; y++) {
		for (unsigned int x = 0, maxX = canvas.getWidth(); x < maxX; x++) {
			Vector rayDirection(1, 2 * real(x) / real(maxX) - 1, (1 - 2 * real(y) / real(maxY)) * 0.75);
			Ray ray(Vector(0, 0, 0), rayDirection, TraceRes::DISTANCE | TraceRes::NORMAL);
			Array<TraceRes> res = sphere.trace(ray);
			
			unsigned char *pix = canvas(x, y);
			if (res.length() == 2) {
				// Calculate the distance travelled inside the sphere.
				real dist = res[1].distance - res[0].distance;
				dist = dist > 0 ? dist : -dist;

				// Calculate the opacity.
				real opacity = 1 - pow(0.9, dist);
				pix[0] = pix[1] = pix[2] = (unsigned char)(255.9 * opacity);
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

