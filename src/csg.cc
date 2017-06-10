#include "csg.h"

#include <algorithm>
#include <iterator>

void Union::Add(const Shape* shape) { contents_.push_back(shape); }

void Union::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  const size_t start_index = output->size();

  // CSG requires additional features, if they are not already enabled.
  Ray ray_copy = ray;
  ray_copy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  bool inside_union = false;
  for (const Shape* shape : contents_) {
    const size_t middle_index = output->size();

    // Trace the boundaries of the underlying shape.
    shape->Trace(ray_copy, output);

    // Merge the boundaries.
    std::inplace_merge(output->begin() + start_index,
                       output->begin() + middle_index,
                       output->end());

    // Compute the initial depth of the overlapping regions.
    bool inside_shape = shape->Contains(ray_copy.start);
    int depth = inside_union + inside_shape;

    // Compute the union from the merged boundaries.
    auto is_internal_boundary = [&](const TraceRes& entry) {
      depth = entry.entering ? depth + 1 : depth - 1;
      if (depth == 1 && entry.entering) return false;  // Entering the union.
      if (depth == 0 && !entry.entering) return false;  // Leaving the union.
      return true;  // Neither entering nor leaving, so internal.
    };
    auto new_end = std::remove_if(output->begin() + start_index, output->end(),
                                  is_internal_boundary);
    output->erase(new_end, output->end());

    // Update the starting state for the ray.
    inside_union = inside_union || inside_shape;
  }
}

bool Union::Contains(Vector point) const {
  auto contains_point = [&](const Shape* shape) {
    return shape->Contains(point);
  };
  return std::any_of(contents_.begin(), contents_.end(), contains_point);
}

const char* Union::Name() const { return "Union"; }

void Intersection::Add(const Shape* shape) { contents_.push_back(shape); }

void Intersection::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  const size_t start_index = output->size();

  // CSG requires additional features, if they are not already enabled.
  Ray ray_copy = ray;
  ray_copy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  std::vector<TraceRes> boundaries;
  bool inside_intersection = true;
  for (const Shape* shape : contents_) {
    const size_t middle_index = output->size();

    // Trace the boundaries of the underlying shape.
    shape->Trace(ray_copy, output);
    bool inside_shape = shape->Contains(ray_copy.start);

    if (output->size() == middle_index && !inside_shape) {
      // If the ray was entirely outside the shape, the whole intersection is
      // empty. Remove the entire intermediate union from the output and return.
      output->erase(output->begin() + start_index, output->end());
      return;
    }

    // Merge the boundaries.
    std::inplace_merge(output->begin() + start_index,
                       output->begin() + middle_index,
                       output->end());

    // Compute the initial depth of the overlapping regions.
    int depth = inside_intersection + inside_shape;

    // Compute the intersection from the merged boundaries.
    auto is_external_boundary = [&](const TraceRes& entry) {
      depth = entry.entering ? depth + 1 : depth - 1;
      if (depth == 2 && entry.entering) return false;  // Entering intersection.
      if (depth == 1 && !entry.entering) return false;  // Leaving intersection.
      return true;  // Neither entering nor leaving, so external.
    };
    auto new_end = std::remove_if(output->begin() + start_index, output->end(),
                                  is_external_boundary);
    output->erase(new_end, output->end());

    // Update the starting state for the ray.
    inside_intersection = inside_intersection && inside_shape;
  }
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

void Complement::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  // CSG requires additional features, if they are not already enabled.
  Ray ray_copy = ray;
  ray_copy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  const size_t start_index = output->size();
  shape_->Trace(ray_copy, output);

  auto flip_boundary = [&](TraceRes& boundary) {
    boundary.entering = !boundary.entering;
    boundary.normal = -boundary.normal;
  };
  std::for_each(output->begin() + start_index, output->end(), flip_boundary);
}

bool Complement::Contains(Vector point) const {
  return !shape_->Contains(point);
}

const char* Complement::Name() const { return "Complement"; }
