#pragma once

#include "camera.h"
#include "image.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

#include <mutex>
#include <thread>

class Render {
 private:
  Vector (*renderPixel)(Shape *, Ray);

  unsigned int currentChunk, numChunks, numPasses;

  Vector *buffer;
  Image output;

  Camera cam;
  Shape *shape;

  std::mutex mtx;

 public:
  unsigned int subPixels, numThreads, chunkWidth, chunkHeight;
  real brightness, contrast;

 public:
  Render(Vector (*)(Shape *, Ray), Shape *, Camera);
  ~Render();

  static void RenderChunk(Render *);

  Image operator()();
};
