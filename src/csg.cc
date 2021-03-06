#include "csg.h"

#include <algorithm>
#include <iterator>

namespace {

// This stands in for inplace_merge. However, it maintains and reuses a single
// buffer per thread to avoid performing many allocations.
template <typename Iterator>
void Merge(Iterator first, Iterator middle, Iterator last) {
  thread_local std::vector<typename Iterator::value_type> buffer;
  buffer.clear();
  std::merge(first, middle, middle, last, back_inserter(buffer));
  std::copy(buffer.begin(), buffer.end(), first);
}

}  // namespace

void Union::Add(std::unique_ptr<Shape> shape) {
  contents_.push_back(std::move(shape));
}

void Union::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  const size_t start_index = output->size();

  bool inside_union = false;
  for (const auto& shape : contents_) {
    const size_t middle_index = output->size();

    // Trace the boundaries of the underlying shape.
    shape->Trace(ray, output);

    // Merge the boundaries.
    Merge(output->begin() + start_index, output->begin() + middle_index,
          output->end());

    // Compute the initial depth of the overlapping regions.
    bool inside_shape = shape->Contains(ray.start);
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
  auto contains_point = [&](auto& shape) { return shape->Contains(point); };
  return std::any_of(contents_.begin(), contents_.end(), contains_point);
}

void Intersection::Add(std::unique_ptr<Shape> shape) {
  contents_.push_back(std::move(shape));
}

void Intersection::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  const size_t start_index = output->size();

  std::vector<TraceRes> boundaries;
  bool inside_intersection = true;
  for (const auto& shape : contents_) {
    const size_t middle_index = output->size();

    // Trace the boundaries of the underlying shape.
    shape->Trace(ray, output);
    bool inside_shape = shape->Contains(ray.start);

    if (output->size() == middle_index && !inside_shape) {
      // If the ray was entirely outside the shape, the whole intersection is
      // empty. Remove the entire intermediate union from the output and return.
      output->erase(output->begin() + start_index, output->end());
      return;
    }

    // Merge the boundaries.
    Merge(output->begin() + start_index, output->begin() + middle_index,
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
  auto contains_point = [&](auto& shape) { return shape->Contains(point); };
  return std::all_of(contents_.begin(), contents_.end(), contains_point);
}

Complement::Complement(std::unique_ptr<Shape> shape)
    : shape_(std::move(shape)) {}

void Complement::Trace(const Ray& ray, std::vector<TraceRes>* output) const {
  const size_t start_index = output->size();
  shape_->Trace(ray, output);

  auto flip_boundary = [&](TraceRes& boundary) {
    boundary.entering = !boundary.entering;
    boundary.normal = -boundary.normal;
  };
  std::for_each(output->begin() + start_index, output->end(), flip_boundary);
}

bool Complement::Contains(Vector point) const {
  return !shape_->Contains(point);
}
