#include "csg.h"

#include <algorithm>
#include <iterator>

void Union::Add(const Shape* shape) { contents_.push_back(shape); }

std::vector<TraceRes> Union::Trace(const Ray& ray) const {
  // CSG requires additional features, if they are not already enabled.
  Ray ray_copy = ray;
  ray_copy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  std::vector<TraceRes> boundaries;
  bool inside_union = false;
  for (const Shape* shape : contents_) {
    // Trace the boundaries of the underlying shape.
    std::vector<TraceRes> shape_boundaries = shape->Trace(ray_copy);
    std::vector<TraceRes> merged_boundaries;
    std::merge(boundaries.begin(), boundaries.end(),
               shape_boundaries.begin(), shape_boundaries.end(),
               std::back_inserter(merged_boundaries));

    // Compute the initial depth of the overlapping regions.
    bool inside_shape = shape->Contains(ray_copy.start);
    int depth = inside_union + inside_shape;

    // Compute the boundaries of the union.
    boundaries.clear();
    for (const TraceRes& entry : merged_boundaries) {
      if (depth == 0 && entry.entering) {
        // Entering the union of the shapes.
        depth = 1;
        boundaries.push_back(entry);
      } else if (depth == 1 && !entry.entering) {
        // Leaving the union of the shapes.
        depth = 0;
        boundaries.push_back(entry);
      } else {
        // Internal boundary.
        depth = entry.entering ? depth + 1 : depth - 1;
      }
    }

    // Update the starting state for the ray.
    inside_union = inside_union || inside_shape;
  }

  return boundaries;
}

bool Union::Contains(Vector point) const {
  auto contains_point = [&](const Shape* shape) {
    return shape->Contains(point);
  };
  return std::any_of(contents_.begin(), contents_.end(), contains_point);
}

const char* Union::Name() const { return "Union"; }

void Intersection::Add(const Shape* shape) { contents_.push_back(shape); }

std::vector<TraceRes> Intersection::Trace(const Ray& ray) const {
  // CSG requires additional features, if they are not already enabled.
  Ray ray_copy = ray;
  ray_copy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  std::vector<TraceRes> boundaries;
  bool inside_intersection = true;
  for (const Shape* shape : contents_) {
    // Trace the boundaries of the underlying shape.
    std::vector<TraceRes> shape_boundaries = shape->Trace(ray_copy);
    bool inside_shape = shape->Contains(ray_copy.start);

    // If the ray was entirely outside the shape, the whole intersection is
    // empty.
    if (shape_boundaries.empty() && !inside_shape) return {};

    std::vector<TraceRes> merged_boundaries;
    std::merge(boundaries.begin(), boundaries.end(),
               shape_boundaries.begin(), shape_boundaries.end(),
               std::back_inserter(merged_boundaries));

    // Compute the initial depth of the overlapping regions.
    int depth = inside_intersection + inside_shape;

    // Compute the boundaries of the intersection.
    boundaries.clear();
    for (const TraceRes& entry : merged_boundaries) {
      if (depth == 1 && entry.entering) {
        // Entering the intersection of the shapes.
        depth = 2;
        boundaries.push_back(entry);
      } else if (depth == 2 && !entry.entering) {
        // Leaving the intersection of the shapes.
        depth = 1;
        boundaries.push_back(entry);
      } else {
        // External boundary.
        depth = entry.entering ? depth + 1 : depth - 1;
      }
    }

    // Update the starting state for the ray.
    inside_intersection = inside_intersection && inside_shape;
  }

  return boundaries;
}

bool Intersection::Contains(Vector point) const {
  auto contains_point = [&](const Shape* shape) {
    return shape->Contains(point);
  };
  return std::all_of(contents_.begin(), contents_.end(), contains_point);
}

const char* Intersection::Name() const { return "Intersection"; }

Complement::Complement(const Shape* shape)
    : shape_(shape) {}

std::vector<TraceRes> Complement::Trace(const Ray& ray) const {
  // CSG requires additional features, if they are not already enabled.
  Ray ray_copy = ray;
  ray_copy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  std::vector<TraceRes> boundaries = shape_->Trace(ray_copy);
  for (TraceRes& boundary : boundaries) {
    boundary.entering = !boundary.entering;
    boundary.normal = -boundary.normal;
  }

  return boundaries;
}

bool Complement::Contains(Vector point) const {
  return !shape_->Contains(point);
}

const char* Complement::Name() const { return "Complement"; }
