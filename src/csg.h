#pragma once

#include "shape.h"

#include <vector>

class Union : public Shape {
 public:
  void Add(const Shape* shape);

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;

 private:
  std::vector<const Shape*> contents_;
};

class Intersection : public Shape {
 public:
  void Add(const Shape* shape);

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;

 private:
  std::vector<const Shape*> contents_;
};

class Complement : public Shape {
 public:
  Complement(const Shape* shape);

  std::vector<TraceRes> Trace(const Ray& ray) const override;
  bool Contains(Vector point) const override;
  const char* Name() const override;

 private:
  const Shape* shape_;
};
