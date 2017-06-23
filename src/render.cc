#include "render.h"
#include "random.h"
#include "ray.h"

#include <algorithm>
#include <vector>

namespace {

std::uniform_real_distribution<real> real_rand(0, 1);

int DivideAndCeil(int x, int y) { return (x + y - 1) / y; }

}  // namespace

Render::Render(RenderPixelFunction* render_pixel, const Shape* scene,
               Camera camera, RenderOptions options)
    : render_pixel_(render_pixel),
      scene_(scene),
      camera_(camera),
      options_(options),
      num_chunks_(DivideAndCeil(camera_.width(), options_.chunk_width) *
                  DivideAndCeil(camera_.height(), options_.chunk_height)),
      output_(camera_.width(), camera_.height()),
      pixel_colors_(
          std::make_unique<Vector[]>(camera_.width() * camera_.height())) {}

Image Render::operator()() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    next_chunk_ = 0;
    num_passes_++;
  }

  std::vector<std::thread> threads;
  for (int i = 0; i < options_.num_threads; i++)
    threads.push_back(std::thread(&Render::RenderChunk, this));
  for (auto& thread : threads) thread.join();

  return output_;
}

void Render::RenderChunk() {
  int chunks_per_row = DivideAndCeil(camera_.width(), options_.chunk_width);
  while (1) {
    int chunk;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (next_chunk_ == num_chunks_) return;  // All chunks are rendered.
      chunk = next_chunk_;
      next_chunk_++;
    }

    // Calculate the chunk coordinates.
    int chunk_x = options_.chunk_width * (chunk % chunks_per_row);
    int chunk_y = options_.chunk_height * (chunk / chunks_per_row);
    int w = std::min(camera_.width() - chunk_x, options_.chunk_width);
    int h = std::min(camera_.height() - chunk_y, options_.chunk_height);

    // Render the samples.
    auto chunk_pixel_colors_ = std::make_unique<Vector[]>(w * h);
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        Vector color;

        for (int i = 0, n = options_.sub_pixels; i < n; i++) {
          // Create a ray and trace it.
          Ray ray = camera_.GetRay(chunk_x + x + real_rand(RandomGenerator()),
                                   chunk_y + y + real_rand(RandomGenerator()));
          color = color + render_pixel_(scene_, ray);
        }

        // Take the average of all samples.
        color = color / options_.sub_pixels;

        Vector& pixel = chunk_pixel_colors_[y * w + x];
        pixel = pixel + color;
      }
    }

    // Copy the results back into the master image.
    std::unique_lock<std::mutex> lock(mutex_);
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        // Augment the overall sum for this pixel.
        Vector color = chunk_pixel_colors_[y * w + x];
        Vector& pixel = pixel_colors_[
            camera_.width() * (chunk_y + y) + (chunk_x + x)];
        pixel = pixel + color;
        // Compute the value for the bitmap.
        color = pixel / num_passes_;
        Vector bitmap_color =
            256 * (options_.contrast * color + options_.brightness);
        auto* bitmap_pixel = output_(chunk_x + x, chunk_y + y);
        bitmap_pixel[2] = std::clamp(bitmap_color.x, real{0}, real{255});
        bitmap_pixel[1] = std::clamp(bitmap_color.y, real{0}, real{255});
        bitmap_pixel[0] = std::clamp(bitmap_color.z, real{0}, real{255});
      }
    }
  }
}
