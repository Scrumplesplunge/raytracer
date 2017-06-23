#include "camera.h"
#include "config.h"
#include "die.h"
#include "diffuse.h"
#include "glass.h"
#include "image.h"
#include "light.h"
#include "matrix.h"
#include "mirror.h"
#include "plane.h"
#include "ray.h"
#include "render.h"
#include "sky.h"
#include "sphere.h"
#include "trace_res.h"
#include "vector.h"

#include <atomic>
#include <cmath>
#include <iostream>

Glass glass({0.8, 0.9, 0.9});
Diffuse white(1, {1, 1, 1}), red(1, {1, 0.1, 0}),
    green(1, {0.1, 1, 0});
Mirror mirror({0.9, 0.9, 0.9});
Light light({1, 1, 1});
Sky sky("assets/sky.bmp");

Die die(Matrix({0.9511, 0.3090, 0},
               {-0.3090, 0.9511, 0},
               {0, 0, 1},
               {1, -0.6, 0}),
        &mirror);
Die die2(Matrix({0.9511, -0.3090, 0},
                {0.3090, 0.9511, 0},
                {0, 0, 1},
                {-0.5, 0.6, 0}),
         &glass);

Sphere source_far({2, 0, 2}, 0.5);
Sphere source_left({0, -2, 2}, 0.5);
Sphere source_right({0, 2, 2}, 0.5);

Plane box_floor({0, 0, -0.501}, {0, 0, 1});
Plane box_ceil({0, 0, 3.5}, {0, 0, -1});
Plane box_wall_left({0, 2, 0}, {0, -1, 0});
Plane box_wall_right({0, -2, 0}, {0, 1, 0});
Plane box_wall_far({2, 0, 0}, {-1, 0, 0});
Plane box_wall_behind({-10.1, 0, 0}, {1, 0, 0});

std::atomic<uint64_t> num_rays = 0;
class Scene : public Union {
 public:
  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override {
    num_rays++;
    return Union::Trace(ray, output);
  }
};
Scene room;

Vector raytrace(Shape *scene, Ray ray) {
  std::vector<TraceRes> boundaries;
  scene->Trace(ray, &boundaries);
  if (boundaries.size() == 0) {
    return {1, 0, 1};
  } else {
    const Material& material = *boundaries[0].primitive->material;
    return material.outgoingLight(scene, boundaries[0], -ray.direction, 1);
  }
}

int main() {
  // Test image writing.
  if (!sky.good()) {
    sky.printError();
    return 1;
  }

  source_far.material = source_left.material = source_right.material = &light;
  box_wall_far.material = &white;
  box_ceil.material = box_floor.material = box_wall_behind.material = &white;
  box_wall_left.material = &red;
  box_wall_right.material = &green;

  room.Add(&source_far);
  // room.Add(&source_left);
  // room.Add(&source_right);
  room.Add(&die);
  room.Add(&die2);
  room.Add(&box_floor);
  room.Add(&box_ceil);
  room.Add(&box_wall_left);
  room.Add(&box_wall_right);
  room.Add(&box_wall_far);
  room.Add(&box_wall_behind);

  Camera cam(1920, 1080, 0.4);
  cam.MoveTo({-10, 1, 1.5});
  cam.LookAt({0.75, -0.2, 0});

  Render render(raytrace, &room, cam);
  render.numThreads = std::thread::hardware_concurrency();
  render.subPixels = 1;
  render.brightness = 0;
  render.contrast = 20;

  for (unsigned int i = 0; i < 1; i++) {
    Image canvas(render());

    // Save the image.
    char filename[12] = {'t', 'e', 's', 't', '0', '0',
                         '0', '.', 'b', 'm', 'p', '\0'};
    filename[4] = '0' + i / 100;
    filename[5] = '0' + (i / 10) % 10;
    filename[6] = '0' + i % 10;
    std::cout << "Saving " << filename << " (" << num_rays << " so far).."
              << std::flush;
    if (!canvas.save(filename)) {
      canvas.printError();
      return 1;
    }
    std::cout << " Done." << std::endl;
  }

  return 0;
}
