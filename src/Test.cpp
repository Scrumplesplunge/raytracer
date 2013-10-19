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
#include "Render.h"
#include <cmath>
#include <iostream>

// Bulk shape of the die.
Sphere sphere(Vector(0, 0, 0), 0.7071);
Plane plane0(Vector(0, 0, 0.5), Vector(0, 0, 1));
Plane plane1(Vector(0, 0, -0.5), Vector(0, 0, -1));
Plane plane2(Vector(0, 0.5, 0), Vector(0, 1, 0));
Plane plane3(Vector(0, -0.5, 0), Vector(0, -1, 0));
Plane plane4(Vector(0.5, 0, 0), Vector(1, 0, 0));
Plane plane5(Vector(-0.5, 0, 0), Vector(-1, 0, 0));

Sphere one0_(Vector(0, 0, -0.675), 0.2);
CSGComplement one0(&one0_);

Sphere two0_(Vector(0.675, 0.225, -0.225), 0.2);
Sphere two1_(Vector(0.675, -0.225, 0.225), 0.2);
CSGComplement two0(&two0_);
CSGComplement two1(&two1_);

Sphere three0_(Vector(0.225, -0.675, -0.225), 0.2);
Sphere three1_(Vector(0, -0.675, 0), 0.2);
Sphere three2_(Vector(-0.225, -0.675, 0.225), 0.2);
CSGComplement three0(&three0_);
CSGComplement three1(&three1_);
CSGComplement three2(&three2_);

Sphere four0_(Vector(-0.225, 0.675, -0.225), 0.2);
Sphere four1_(Vector(-0.225, 0.675, 0.225), 0.2);
Sphere four2_(Vector(0.225, 0.675, 0.225), 0.2);
Sphere four3_(Vector(0.225, 0.675, -0.225), 0.2);
CSGComplement four0(&four0_);
CSGComplement four1(&four1_);
CSGComplement four2(&four2_);
CSGComplement four3(&four3_);

Sphere five0_(Vector(-0.675, -0.225, -0.225), 0.2);
Sphere five1_(Vector(-0.675, 0.225, -0.225), 0.2);
Sphere five2_(Vector(-0.675, 0.225, 0.225), 0.2);
Sphere five3_(Vector(-0.675, -0.225, 0.225), 0.2);
Sphere five4_(Vector(-0.675, 0, 0), 0.2);
CSGComplement five0(&five0_);
CSGComplement five1(&five1_);
CSGComplement five2(&five2_);
CSGComplement five3(&five3_);
CSGComplement five4(&five4_);

Sphere six0_(Vector(0.225, 0.225, 0.675), 0.2);
Sphere six1_(Vector(0.225, 0, 0.675), 0.2);
Sphere six2_(Vector(0.225, -0.225, 0.675), 0.2);
Sphere six3_(Vector(-0.225, 0.225, 0.675), 0.2);
Sphere six4_(Vector(-0.225, 0, 0.675), 0.2);
Sphere six5_(Vector(-0.225, -0.225, 0.675), 0.2);
CSGComplement six0(&six0_);
CSGComplement six1(&six1_);
CSGComplement six2(&six2_);
CSGComplement six3(&six3_);
CSGComplement six4(&six4_);
CSGComplement six5(&six5_);

Shape *die_parts[] = {&sphere, &plane0, &plane1, &plane2, &plane3, &plane4, &plane5, &one0, &two0, &two1, &three0, &three1, &three2, &four0, &four1, &four2, &four3, &five0, &five1, &five2, &five3, &five4, &six0, &six1, &six2, &six3, &six4, &six5};
unsigned int num_die_parts = sizeof(die_parts) / sizeof(Shape*);
CSGIntersection die;
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

	for (unsigned int i = 0; i < num_die_parts; i++) {
		die.add(die_parts[i]);
	}

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

