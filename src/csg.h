#pragma once

#include "shape.h"

#include <memory>
#include <vector>

class Union : public Shape {
 public:
  void Add(std::unique_ptr<Shape> shape);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  std::vector<std::unique_ptr<Shape>> contents_;
};

class Intersection : public Shape {
 public:
  void Add(std::unique_ptr<Shape> shape);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  std::vector<std::unique_ptr<Shape>> contents_;
};

class Complement : public Shape {
 public:
  Complement(std::unique_ptr<Shape> shape);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  const std::unique_ptr<Shape> shape_;
};
