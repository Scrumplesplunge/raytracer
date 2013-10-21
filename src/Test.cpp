#include "Config.h"
#include "Image.h"
#include "Vector.h"
#include "Matrix.h"
#include "Ray.h"
#include "Array.h"
#include "Die.h"
#include "Plane.h"
#include "Sphere.h"
#include "CSGUnion.h"
#include "TraceRes.h"
#include "Camera.h"
#include "Render.h"
#include "Glass.h"
#include "Diffuse.h"
#include "Light.h"
#include <cmath>
#include <iostream>

Glass glass(Vector(0.8, 0.9, 0.9));
Diffuse white(0.5, Vector(1, 1, 1)), red(0.5, Vector(1, 0, 0)), green(0.5, Vector(0, 1, 0)), black(1, Vector(0, 0, 0));
Light light(Vector(100, 100, 100));

Die die(Matrix(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)), &white);
Sphere lightSource(Vector(1, 1, 3.5), 0.5);
Plane box_floor(Vector(0, 0, -0.5 - EPSILON), Vector(0, 0, 1));
Plane box_ceil(Vector(0, 0, 3.5), Vector(0, 0, -1));
Plane box_wall_left(Vector(0, 2, 0), Vector(0, -1, 0));
Plane box_wall_right(Vector(0, -2, 0), Vector(0, 1, 0));
Plane box_wall_far(Vector(2, 0, 0), Vector(-1, 0, 0));
Plane box_wall_behind(Vector(-20, 0, 0), Vector(1, 0, 0));
CSGUnion room;

Image sky("assets/sky.bmp");

Vector raytrace(Shape *scene, Ray ray) {
	Array<TraceRes> res(scene->trace(ray));
	if (res.length() == 0) {
		return Vector();
	} else {
		return res[0].primitive->material->outgoingLight(scene, res[0], -ray.direction, 1);
	}
}

int main(int argc, char *args[]) {
	// Test image writing.
	if (!sky.good()) {
		sky.printError();
		return 1;
	}

	lightSource.material = &light;

	box_ceil.material = box_floor.material = box_wall_far.material = &white;
	box_wall_behind.material = &white;
	box_wall_left.material = &red;
	box_wall_right.material = &green;

	room.add(&lightSource);
	room.add(&die);
	room.add(&box_floor);
	room.add(&box_ceil);
	room.add(&box_wall_left);
	room.add(&box_wall_right);
	room.add(&box_wall_far);
	room.add(&box_wall_behind);

	Camera cam(512, 512, 1.7);
	cam.moveTo(Vector(-3, 0, 0.2));

	Render render(raytrace, &room, cam);
	render.numThreads = 4;
	render.subPixelsX = 1;
	render.subPixelsY = 1;

	for (unsigned int i = 0; i < 1000; i++) {
		Image canvas(render());
		
		// Save the image.
		char filename[12] = {'t', 'e', 's', 't', '0', '0', '0', '.', 'b', 'm', 'p', '\0'};
		filename[4] = '0' + i / 100;
		filename[5] = '0' + (i / 10) % 10;
		filename[6] = '0' + i % 10;
		std::cout << "Saving " << filename << ".. ";
		if (!canvas.save(filename)) {
			canvas.printError();
			return 1;
		}
		std::cout << "Done." << std::endl;
	}

	return 0;
}

