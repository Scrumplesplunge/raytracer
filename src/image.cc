#include "image.h"

#define cimg_display 0  // Disable display capabilities for CImg.
#include <CImg.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

Image::Image(int width, int height)
    : width_(width), height_(height),
      pixels_(std::make_unique<Pixel[]>(width_ * height_)) {}

Image::Image(const Image& other)
    : width_(other.width_), height_(other.height_),
      pixels_(std::make_unique<Pixel[]>(width_ * height_)) {
  std::copy(other.pixels_.get(), other.pixels_.get() + width_ * height_,
            pixels_.get());
}

Image Image::Load(const std::string& filename) {
  const cimg_library::CImg<Pixel::Component> file(filename.c_str());
  Image image(file.width(), file.height());
  for (int y = 0; y < file.height(); y++) {
    for (int x = 0; x < file.width(); x++) {
      image(x, y) =
          {*file.data(x, y, 0), *file.data(x, y, 1), *file.data(x, y, 2)};
    }
  }
  return image;
}

void Image::Save(const std::string& filename) const {
  cimg_library::CImg<unsigned char> file(width_, height_, 1, 3);
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      file(x, y, 0) = operator()(x, y).red;
      file(x, y, 1) = operator()(x, y).green;
      file(x, y, 2) = operator()(x, y).blue;
    }
  }
  file.save(filename.c_str());
}

Pixel& Image::operator()(int x, int y) {
  if (x < 0 || width_ <= x || y < 0 || height_ <= y)
    throw std::runtime_error("Pixel coordinate out of bounds.");
  return pixels_[y * width_ + x];
}

Pixel Image::operator()(int x, int y) const {
  if (x < 0 || width_ <= x || y < 0 || height_ <= y)
    throw std::runtime_error("Pixel coordinate out of bounds.");
  return pixels_[y * width_ + x];
}
