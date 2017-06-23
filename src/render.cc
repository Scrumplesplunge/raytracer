#include "render.h"
#include "random.h"
#include "ray.h"

#include <algorithm>
#include <vector>

std::uniform_real_distribution<real> real_rand(0, 1);

Render::Render(Vector (*renderPixelFunc)(Shape *, Ray), Shape *s, Camera c)
    : renderPixel(renderPixelFunc),
      currentChunk(0),
      numChunks(0),
      numPasses(0),
      buffer(new Vector[c.width() * c.height()]),
      output(c.width(), c.height()),
      cam(c),
      shape(s),
      subPixels(1),
      numThreads(1),
      chunkWidth(64),
      chunkHeight(64),
      brightness(0),
      contrast(1) {}

Render::~Render() { delete[] buffer; }

Image Render::operator()() {
  numChunks = ((cam.width() + chunkWidth - 1) / chunkWidth) *
              ((cam.height() + chunkHeight - 1) / chunkHeight);
  currentChunk = 0;
  numPasses++;

  std::vector<std::thread> threads;
  for (unsigned int i = 0; i < numThreads; i++)
    threads.push_back(std::thread(&Render::RenderChunk, this));
  for (auto& thread : threads) thread.join();

  return output;
}

void Render::RenderChunk(Render *parent) {
  while (1) {
    parent->mtx.lock();
    // All chunks are taken.  Exit.
    if (parent->currentChunk == parent->numChunks) {
      parent->mtx.unlock();
      return;
    }
    unsigned int chunk = parent->currentChunk;
    parent->currentChunk++;
    parent->mtx.unlock();

    // Calculate the chunk coordinates.
    unsigned int chunksPerRow =
        (parent->cam.width() + parent->chunkWidth - 1) / parent->chunkWidth;
    unsigned int x = parent->chunkWidth * (chunk % chunksPerRow);
    unsigned int y = parent->chunkHeight * (chunk / chunksPerRow);
    unsigned int x2 = std::min(x + parent->chunkWidth, parent->cam.width());
    unsigned int y2 = std::min(y + parent->chunkHeight, parent->cam.height());

    Image output(x2 - x, y2 - y);

    for (unsigned int yi = y; yi < y2; yi++) {
      for (unsigned int xi = x; xi < x2; xi++) {
        Vector color;

        // Supersampling.
        if (parent->subPixels == 0) {
          color = parent->renderPixel(
              parent->shape,
              parent->cam.GetRay(xi + 0.5, yi + 0.5));
        } else {
          for (unsigned int i = 0, im = parent->subPixels; i < im; i++) {
            // Create a ray and trace it.
            Ray ray = parent->cam.GetRay(xi + real_rand(RandomGenerator()),
                                         yi + real_rand(RandomGenerator()));
            color = color + parent->renderPixel(parent->shape, ray);
          }

          // Take the average of all samples.
          color = color / parent->subPixels;
        }

        Vector *ptr = parent->buffer + parent->cam.width() * yi + xi;

        // Store the sum of all colours; used for calculating fine average over
        // time.
        ptr[0] = ptr[0] + color;

        // Output the current average to the pixel.
        color = 256 * (parent->contrast * (ptr[0] / parent->numPasses) +
                       parent->brightness);
        unsigned char *pix = output(xi - x, yi - y);
        pix[2] =
            (unsigned char)(color.x >= 256 ? 255 : (color.x < 0 ? 0 : color.x));
        pix[1] =
            (unsigned char)(color.y >= 256 ? 255 : (color.y < 0 ? 0 : color.y));
        pix[0] =
            (unsigned char)(color.z >= 256 ? 255 : (color.z < 0 ? 0 : color.z));
      }
    }

    parent->output.draw(output, x, y);
  }
}
