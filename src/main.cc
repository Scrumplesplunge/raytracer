#include "camera.h"
#include "config.h"
#include "die.h"
#include "diffuse.h"
#include "glass.h"
#include "image.h"
#include "light.h"
#include "mirror.h"
#include "ray.h"
#include "render.h"
#include "shapes.h"
#include "sky.h"
#include "trace_res.h"
#include "transform.h"
#include "vector.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>

// The things we want to count happen regularly enough that simply incrementing
// the atomic integer is too slow. This class can be used to accumulate a count
// before adding in bulk. For example, a thread-local instance for counters that
// are only examined once threads have been terminated.
class Counter {
 public:
  Counter(std::atomic<uint64_t>* output) : output_(output) {}
  ~Counter() { *output_ += value_; }

  void operator++(int) { value_++; }

 private:
  std::atomic<uint64_t>* output_;
  uint_fast64_t value_;
};

std::atomic<uint64_t> total_num_rays = 0;
thread_local Counter num_rays{&total_num_rays};

class Scene : public Union {
 public:
  Scene() {
    // Add the dice.
    Add(std::make_unique<Die>(
            glass_.get(), Transform{}.RotateZ(-0.3).Translate({-0.5, 0.6, 0})));
    Add(std::make_unique<Die>(
            mirror_.get(), Transform{}.RotateZ(0.3).Translate({1, -0.6, 0})));
    // Add the walls.
    Add(std::make_unique<Plane>(
            white_.get(), Vector{0, 0, -0.501}, Vector{0, 0, 1}));  // Floor
    Add(std::make_unique<Plane>(
            white_.get(), Vector{0, 0, 3.5}, Vector{0, 0, -1}));  // Ceiling
    Add(std::make_unique<Plane>(
            red_.get(), Vector{0, 2, 0}, Vector{0, -1, 0}));  // Left wall
    Add(std::make_unique<Plane>(
            green_.get(), Vector{0, -2, 0}, Vector{0, 1, 0}));  // Right wall
    Add(std::make_unique<Plane>(
            white_.get(), Vector{2, 0, 0}, Vector{-1, 0, 0}));  // Far wall
    Add(std::make_unique<Plane>(
            white_.get(), Vector{-10.1, 0, 0}, Vector{1, 0, 0}));  // Behind
    // Add the light source.
    Add(std::make_unique<Sphere>(
            light_.get(), Vector{2, 0, 2}, 0.5));  // Far wall
  }

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override {
    num_rays++;
    return Union::Trace(ray, output);
  }

 private:
  std::unique_ptr<Material>
      glass_ = std::make_unique<Glass>(Vector{0.8, 0.9, 0.9}),
      green_ = std::make_unique<Diffuse>(1, Vector{0.1, 1, 0}),
      light_ = std::make_unique<Light>(Vector{1, 1, 1}),
      mirror_ = std::make_unique<Mirror>(Vector{0.9, 0.9, 0.9}),
      red_ = std::make_unique<Diffuse>(1, Vector{1, 0.1, 0}),
      white_ = std::make_unique<Diffuse>(1, Vector{1, 1, 1});
};

Vector Raytrace(const Shape* scene, Ray ray) {
  std::vector<TraceRes> boundaries;
  scene->Trace(ray, &boundaries);
  if (boundaries.size() == 0) {
    return {1, 0, 1};
  } else {
    return boundaries[0].material->OutgoingLight(
        scene, boundaries[0], -ray.direction, 1);
  }
}

std::string Filename(int iteration) {
  constexpr int MAX_LENGTH = 100;
  char buffer[MAX_LENGTH];
  snprintf(buffer, MAX_LENGTH, "frame_%05d.png", iteration);
  return buffer;
}

void SaveImage(const Image& image, int iteration, int num_rays_so_far,
               std::chrono::nanoseconds duration) {
  std::string filename = Filename(iteration);
  std::chrono::milliseconds duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  std::cout << "Saving " << filename << " (" << num_rays_so_far
            << " rays in " << duration_ms.count() << "ms).." << std::flush;
  image.Save(filename);
  std::cout << " Done." << std::endl;
}

int main() {
  Scene scene;
  Camera cam(3840, 2160, 0.4);
  cam.MoveTo({-10, 1, 1.5});
  cam.LookAt({0.75, -0.2, 0});

  RenderOptions options;
  options.sub_pixels = 1;
  options.contrast = 20;

  Render render(Raytrace, &scene, cam, options);

  constexpr int ITERATIONS = 10000;
  using clock = std::chrono::steady_clock;
  const auto start = clock::now();
  Image image = render();
  for (unsigned int i = 0; i < ITERATIONS - 1; i++) {
    int num_rays_so_far = total_num_rays;
    std::thread save_thread([
        start, i, num_rays_so_far, image = std::move(image)] {
      // Saving the image can take a while, so try to do it in parallel.
      SaveImage(image, i, num_rays_so_far, clock::now() - start);
    });
    image = render();
    save_thread.join();
  }
  SaveImage(image, ITERATIONS - 1, total_num_rays, clock::now() - start);

  return 0;
}
