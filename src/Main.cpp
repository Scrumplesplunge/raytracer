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
#include "Sky.h"
#include <cmath>
#include <iostream>

Glass glass(Vector(0.8, 0.9, 0.9));
Diffuse white(1, Vector(1, 1, 1)), red(1, Vector(1, 0.1, 0)), green(1, Vector(0.1, 1, 0));
Light light(Vector(1, 1, 1));
Sky sky("assets/sky.bmp");

Die die(Matrix(Vector(0.9511, 0.3090, 0), Vector(-0.3090, 0.9511, 0), Vector(0, 0, 1), Vector(-1, -0.6, 0)), &glass);
Die die2(Matrix(Vector(0.9511, -0.3090, 0), Vector(0.3090, 0.9511, 0), Vector(0, 0, 1), Vector(-0.5, 0.6, 0)), &white);

Sphere source(Vector(1, 0, 2), 0.5);

Plane box_floor(Vector(0, 0, -0.501), Vector(0, 0, 1));
Plane box_ceil(Vector(0, 0, 3.5), Vector(0, 0, -1));
Plane box_wall_left(Vector(0, 2, 0), Vector(0, -1, 0));
Plane box_wall_right(Vector(0, -2, 0), Vector(0, 1, 0));
Plane box_wall_far(Vector(2, 0, 0), Vector(-1, 0, 0));
Plane box_wall_behind(Vector(-10.1, 0, 0), Vector(1, 0, 0));
CSGUnion room;

Vector raytrace(Shape *scene, Ray ray) {
	Array<TraceRes> res(scene->trace(ray));
	if (res.length() == 0) {
		return Vector(1, 0, 1);
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

	source.material = &light;
	box_wall_far.material = &white;
	box_ceil.material = box_floor.material = box_wall_behind.material = &white;
	box_wall_left.material = &red;
	box_wall_right.material = &green;

	room.add(&source);
	room.add(&die);
	room.add(&die2);
	room.add(&box_floor);
	room.add(&box_ceil);
	room.add(&box_wall_left);
	room.add(&box_wall_right);
	room.add(&box_wall_far);
	room.add(&box_wall_behind);

	Camera cam(3840, 2160, 0.4);
	cam.moveTo(Vector(-10, 1, 1.5));
	cam.lookAt(Vector(0.75, -0.2, 0));

	Render render(raytrace, &room, cam);
	render.numThreads = 16;
	render.subPixels = 10;
	render.brightness = 0;
	render.contrast = 5;

	for (unsigned int i = 0; i < 100; i++) {
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

