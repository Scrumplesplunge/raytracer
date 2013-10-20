#include "Config.h"
#include "Image.h"
#include "Vector.h"
#include "Matrix.h"
#include "Ray.h"
#include "Array.h"
#include "Die.h"
#include "TraceRes.h"
#include "Camera.h"
#include "Render.h"
#include <cmath>
#include <iostream>

Die die(Matrix(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)));
Image sky("assets/sky.bmp");

Vector raytrace(Shape *shape, Ray ray, unsigned int limit) {
	if (limit == 0) return Vector();

	// Add requirements to the ray.
	ray.mask |= TraceRes::POSITION | TraceRes::NORMAL | TraceRes::ENTERING;

	Array<TraceRes> res(shape->trace(ray));
	if (res.length() == 0) {
		// Calculate colour of skybox in this direction.
		Vector flatDir = ray.direction.setZ(0).normalized();
		unsigned int u = (unsigned int)((sky.getWidth() - 0.5) * (flatDir.y < 0 ? acos(flatDir.x) : TWOPI - acos(flatDir.x)) / TWOPI);
		unsigned int v = (unsigned int)((sky.getHeight() - 0.5) * acos(ray.direction.z) / PI);
		unsigned char *skypix = sky(u, v);
		return Vector(real(skypix[2]) / 255, real(skypix[1]) / 255, real(skypix[0]) / 255);
	} else {
		// Perform refraction on the ray.
		// direction2 = direction + (refractive index 2 - refractive index 1) * normal towards direction
		// Refractivity of glass - refractivity of air.
		real mul = 1.5 - 1;
		if (res[0].entering) {
			// 0.5 * -normal
			Ray newRay(res[0].position - res[0].normal * EPSILON, ray.direction - mul * res[0].normal, ray.mask);
			return raytrace(shape, newRay, limit - 1);
		} else {
			// -0.5 * normal
			Ray newRay(res[0].position + res[0].normal * EPSILON, ray.direction - mul * res[0].normal, ray.mask);
			Vector temp = raytrace(shape, newRay, limit - 1);

			// Make the glass green inside.
			return Vector(temp.x * pow(0.8, res[0].distance), temp.y * pow(0.9, res[0].distance), temp.z * pow(0.9, res[0].distance));
		}
	}
}

Vector raytrace(Shape *shape, Ray ray) {
	return raytrace(shape, ray, 10);
}

int main(int argc, char *args[]) {
	// Test image writing.
	if (!sky.good()) {
		sky.printError();
		return 1;
	}

	Camera cam(512, 512, 1);

	Render render(raytrace);
	render.numThreads = 4;
	
	real theta = 0;
	for (unsigned int i = 0; i < 100; i++) {
		theta += TWOPI / 100;
		real c = cos(theta), s = sin(theta);
		cam.moveTo(Vector(-2 * c, -2 * s, 0));
		cam.lookAt(Vector());

		Image canvas(render(&die, &cam));
	
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

