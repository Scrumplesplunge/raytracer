#pragma once

#include "shape.h"

#include <vector>

class Union : public Shape {
 public:
  void Add(const Shape* shape);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  std::vector<const Shape*> contents_;
};

class Intersection : public Shape {
 public:
  void Add(const Shape* shape);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  std::vector<const Shape*> contents_;
};

class Complement : public Shape {
 public:
  Complement(const Shape* shape);

  void Trace(const Ray& ray, std::vector<TraceRes>* output) const override;
  bool Contains(Vector point) const override;

 private:
  const Shape* shape_;
};
