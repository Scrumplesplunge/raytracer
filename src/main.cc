#include "camera.h"
#include "config.h"
#include "csg_union.h"
#include "die.h"
#include "diffuse.h"
#include "glass.h"
#include "image.h"
#include "light.h"
#include "matrix.h"
#include "plane.h"
#include "ray.h"
#include "render.h"
#include "sky.h"
#include "sphere.h"
#include "trace_res.h"
#include "vector.h"

#include <cmath>
#include <iostream>

Glass glass({0.8, 0.9, 0.9});
Diffuse white(1, {1, 1, 1}), red(1, {1, 0.1, 0}),
    green(1, {0.1, 1, 0});
Light light({1, 1, 1});
Sky sky("assets/sky.bmp");

Die die(Matrix({0.9511, 0.3090, 0},
               {-0.3090, 0.9511, 0},
               {0, 0, 1},
               {1, -0.6, 0}),
        &white);
Die die2(Matrix({0.9511, -0.3090, 0},
                {0.3090, 0.9511, 0},
                {0, 0, 1},
                {-0.5, 0.6, 0}),
         &glass);

Sphere source({1, 0, 2}, 0.5);

Plane box_floor({0, 0, -0.501}, {0, 0, 1});
Plane box_ceil({0, 0, 3.5}, {0, 0, -1});
Plane box_wall_left({0, 2, 0}, {0, -1, 0});
Plane box_wall_right({0, -2, 0}, {0, 1, 0});
Plane box_wall_far({2, 0, 0}, {-1, 0, 0});
Plane box_wall_behind({-10.1, 0, 0}, {1, 0, 0});
CSGUnion room;

Vector raytrace(Shape *scene, Ray ray) {
  std::vector<TraceRes> res(scene->trace(ray));
  if (res.size() == 0) {
    return {1, 0, 1};
  } else {
    return res[0].primitive->material->outgoingLight(scene, res[0],
                                                     -ray.direction, 1);
  }
}

int main() {
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
  cam.MoveTo({-10, 1, 1.5});
  cam.LookAt({0.75, -0.2, 0});

  Render render(raytrace, &room, cam);
  render.numThreads = 8;
  render.subPixels = 1;
  render.brightness = 0;
  render.contrast = 10;

  for (unsigned int i = 0; i < 100; i++) {
    Image canvas(render());

    // Save the image.
    char filename[12] = {'t', 'e', 's', 't', '0', '0',
                         '0', '.', 'b', 'm', 'p', '\0'};
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
