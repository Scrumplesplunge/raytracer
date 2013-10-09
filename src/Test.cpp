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
#include "Camera.h"
#include <cmath>
#include <iostream>

Sphere sphere(Vector(0, 0, 0), 3);
Sphere sphere2(Vector(-10, 0, 0), 9);
Sphere sphere3(Vector(10, 0, 0), 9);
Sphere sphere4(Vector(0, 0, 0), 2.75);
Plane clip(Vector(-2, 0, 0), Vector(-1, 0, 0));
CSGComplement antisphere1(&sphere2);
CSGComplement antisphere2(&sphere3);
CSGComplement antisphere3(&sphere4);
CSGIntersection both;

int main(int argc, char *args[]) {
	// Test image writing.
	Image canvas(512, 512);
	Camera cam(512, 512, 1);

	both.add(&sphere);
	both.add(&clip);
	both.add(&antisphere1);
	both.add(&antisphere2);
	both.add(&antisphere3);

	real theta = 0;
	for (unsigned int i = 0; i < 50; i++) {
		theta += PI / 50;
		real c = cos(theta), s = sin(theta);
		cam.moveTo(Vector(-7 * c, -7 * s, 0));
		cam.lookAt(Vector());

		for (unsigned int y = 0, maxY = canvas.getHeight(); y < maxY; y++) {
			for (unsigned int x = 0, maxX = canvas.getWidth(); x < maxX; x++) {
				Ray ray(cam.getRay(real(x) + 0.5, real(y) + 0.5, TraceRes::DISTANCE | TraceRes::ENTERING));
				Array<TraceRes> res = both.trace(ray);

				real dist = 0;
				if (res.length() > 0) {
					if (res[0].entering) {
						for (unsigned int i = 1; i < res.length(); i += 2) dist += res[i].distance - res[i - 1].distance;
						if (res.length() & 1) dist += HUGE;
					} else {
						dist += res[0].distance;
						for (unsigned int i = 2; i < res.length(); i += 2) dist += res[i].distance - res[i - 1].distance;
						if (res.length() & 1 == 0) dist += HUGE;
					}
				}
				unsigned char shade = (unsigned char)(255.9 * (1 - pow(0.5, dist)));
				
				unsigned char *pix = canvas(x, y);
				pix[0] = pix[1] = pix[2] = shade;
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

