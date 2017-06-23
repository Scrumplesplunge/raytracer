#pragma once

#include <cstdint>
#include <memory>
#include <string>

struct Pixel {
  using Component = std::uint8_t;
  Component red = 0;
  Component green = 0;
  Component blue = 0;
};

class Image {
 public:
  Image(int width, int height);
  Image(const Image& other);
  Image(Image&& other) = default;
  Image& operator=(Image&& other) = default;

  static Image Load(const std::string& filename);
  void Save(const std::string& filename) const;

  int width() const { return width_; }
  int height() const { return height_; }

  Pixel& operator()(int x, int y);
  Pixel operator()(int x, int y) const;

 private:
  int width_, height_;
  std::unique_ptr<Pixel[]> pixels_;
};
