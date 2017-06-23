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
#include <iostream>
#include <string>

Glass glass({0.8, 0.9, 0.9});
Diffuse white(1, {1, 1, 1}), red(1, {1, 0.1, 0}),
    green(1, {0.1, 1, 0});
Mirror mirror({0.9, 0.9, 0.9});
Light light({1, 1, 1});
Sky sky("assets/sky.bmp");

Die die(Matrix{{0.9511, 0.3090, 0},
               {-0.3090, 0.9511, 0},
               {0, 0, 1},
               {1, -0.6, 0}},
        &mirror);
Die die2(Matrix{{0.9511, -0.3090, 0},
                {0.3090, 0.9511, 0},
                {0, 0, 1},
                {-0.5, 0.6, 0}},
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

Vector Raytrace(const Shape* scene, Ray ray) {
  std::vector<TraceRes> boundaries;
  scene->Trace(ray, &boundaries);
  if (boundaries.size() == 0) {
    return {1, 0, 1};
  } else {
    const Material& material = *boundaries[0].primitive->material;
    return material.OutgoingLight(scene, boundaries[0], -ray.direction, 1);
  }
}

std::string Filename(int iteration) {
  constexpr int MAX_LENGTH = 100;
  char buffer[MAX_LENGTH];
  snprintf(buffer, MAX_LENGTH, "frame_%05d.png", iteration);
  return buffer;
}

int main() {
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

  Camera cam(640, 480, 0.4);
  cam.MoveTo({-10, 1, 1.5});
  cam.LookAt({0.75, -0.2, 0});

  RenderOptions options;
  options.sub_pixels = 1;
  options.contrast = 20;

  Render render(Raytrace, &room, cam, options);

  for (unsigned int i = 0; i < 100; i++) {
    Image canvas(render());

    // Save the image.
    std::string filename = Filename(i);
    std::cout << "Saving " << filename << " (" << num_rays << " so far).."
              << std::flush;
    canvas.Save(filename);
    std::cout << " Done." << std::endl;
  }

  return 0;
}
