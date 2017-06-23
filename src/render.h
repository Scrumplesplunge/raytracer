#pragma once

#include "camera.h"
#include "image.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

#include <cstdint>
#include <mutex>
#include <thread>

struct RenderOptions {
  real brightness = 0;
  real contrast = 1;
  int sub_pixels = 1;
  int num_threads = std::thread::hardware_concurrency();
  int chunk_width = 64;
  int chunk_height = 64;
};

class Render {
 public:
  using RenderPixelFunction = Vector(const Shape*, Ray);

  Render(RenderPixelFunction* render_pixel, const Shape* scene, Camera camera,
         RenderOptions options = {});

  Image operator()();

 private:
  void RenderChunk();

  const RenderPixelFunction* const render_pixel_;
  const Shape* scene_;
  const Camera camera_;
  const RenderOptions options_;
  int num_chunks_ = 0;

  std::mutex mutex_;
  int next_chunk_ = 0;
  int num_passes_ = 0;
  Image output_;
  std::unique_ptr<Vector[]> pixel_colors_;
};
