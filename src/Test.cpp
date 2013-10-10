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
Image canvas(512, 512);
Image sky("assets/sky.bmp");

void raytrace(Ray ray, unsigned char *pix, unsigned int limit = 10) {
	if (limit == 0) {
		// Total internal reflection.
		pix[0] = pix[1] = pix[2] = 0;
		return;
	}

	// Add requirements to the ray.
	ray.mask |= TraceRes::POSITION | TraceRes::NORMAL | TraceRes::ENTERING;

	Array<TraceRes> res(die.trace(ray));
	if (res.length() == 0) {
		
		// Calculate colour of skybox in this direction.
		Vector flatDir = ray.direction.setZ(0).normalized();
		unsigned int u = (unsigned int)((sky.getWidth() - 0.5) * (flatDir.y > 0 ? acos(flatDir.x) : TWOPI - acos(flatDir.x)) / TWOPI);
		unsigned int v = (unsigned int)((sky.getHeight() - 0.5) * acos(ray.direction.z) / PI);
		unsigned char *skypix = sky(u, v);
		pix[0] = skypix[0];
		pix[1] = skypix[1];
		pix[2] = skypix[2];
		
		//pix[0] = (unsigned char)(127.9 * (1 + ray.direction.x));
		//pix[1] = (unsigned char)(127.9 * (1 + ray.direction.y));
		//pix[2] = (unsigned char)(127.9 * (1 + ray.direction.z));
	} else {
		// Perform refraction on the ray.
		// direction2 = direction + (refractive index 2 - refractive index 1) * normal towards direction
		// Refractivity of glass - refractivity of air.
		real mul = 1.5 - 1;
		if (res[0].entering) {
			// 0.5 * -normal
			Ray newRay(res[0].position - res[0].normal * EPSILON, ray.direction - mul * res[0].normal, ray.mask);
			return raytrace(newRay, pix, limit - 1);
		} else {
			// -0.5 * normal
			Ray newRay(res[0].position + res[0].normal * EPSILON, ray.direction - mul * res[0].normal, ray.mask);
			raytrace(newRay, pix, limit - 1);

			// Make the glass green inside.
			real lerp = 1 - pow(0.5, res[0].distance);
			pix[0] = (unsigned char)(pix[0] * (1 - 0.99 * lerp));
			pix[1] = (unsigned char)(pix[1] * (1 - 0.55 * lerp));
			pix[2] = (unsigned char)(pix[2] * (1 - 0.6 * lerp));
		}
	}
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
	
	real theta = 0;
	for (unsigned int i = 0; i < 100; i++) {
		theta += TWOPI / 100;
		real c = cos(theta), s = sin(theta);
		cam.moveTo(Vector(-2 * c, -2 * s, 0));
		cam.lookAt(Vector());

		for (unsigned int y = 0, maxY = canvas.getHeight(); y < maxY; y++) {
			for (unsigned int x = 0, maxX = canvas.getWidth(); x < maxX; x++) {
				Ray ray(cam.getRay(real(x) + 0.5, real(y) + 0.5, TraceRes::DISTANCE | TraceRes::ENTERING | TraceRes::NORMAL));
				raytrace(ray, canvas(x, y));
				/*
				Array<TraceRes> res = die.trace(ray);

				real dist = 0;
				unsigned char *pix = canvas(x, y);
				if (res.length() > 0) {
					pix[0] = (unsigned char)(127.9 * (res[0].normal.x + 1));
					pix[1] = (unsigned char)(127.9 * (res[0].normal.y + 1));
					pix[2] = (unsigned char)(127.9 * (res[0].normal.z + 1));
				} else {
					pix[0] = pix[1] = pix[2] = 0;
				}
				

				// OR //
				
					
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
				
				pix[0] = pix[1] = pix[2] = shade;
				*/
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

