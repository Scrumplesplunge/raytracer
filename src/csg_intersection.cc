#include "csg_intersection.h"
#include "config.h"

#include <algorithm>
#include <iterator>

CSGIntersection::CSGIntersection() {}

void CSGIntersection::add(const Shape* shape) { contents.push_back(shape); }

std::vector<TraceRes> CSGIntersection::Trace(const Ray& ray) const {
  Ray rayCopy(ray);

  // These are required for CSG.
  rayCopy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  if (contents.size() == 0) return {};

  std::vector<TraceRes> boundaries = contents[0]->Trace(rayCopy);

  // Keep track of whether the start is inside the CSGIntersection shape.
  bool inside = contents[0]->Contains(rayCopy.start);

  // If there are no intersections and we aren't inside, then there is no
  // intersection at all.
  if (boundaries.size() == 0 && !inside) return boundaries;

  for (unsigned int i = 1; i < contents.size(); i++) {
    // Results from one branch, merged in with existing output.
    std::vector<TraceRes> branch_boundaries = contents[i]->Trace(rayCopy);
    bool isContained = contents[i]->Contains(rayCopy.start);

    if (branch_boundaries.size() == 0) {
      // Intersect with universe is identity.
      if (isContained) continue;

      // Intersect with empty set is empty set.
      return branch_boundaries;
    }

    std::vector<TraceRes> merged_boundaries;
    std::merge(boundaries.begin(), boundaries.end(),
               branch_boundaries.begin(), branch_boundaries.end(),
               std::back_inserter(merged_boundaries));

    // Meh, this vaguely resembles a semaphore.
    unsigned int sem = 0;

    // Set the initial state of the semaphore.
    if (inside) sem++;
    if (isContained) sem++;
    inside = inside && isContained;

    // Empty the output array, and generate the new one.
    boundaries.clear();
    for (const TraceRes& entry : merged_boundaries) {
      if (sem == 2) boundaries.push_back(entry);
      if (entry.entering) {
        sem++;
      } else {
        sem--;
      }
      if (sem == 2) boundaries.push_back(entry);
    }
  }

  // Return the result.
  return boundaries;
}

bool CSGIntersection::Contains(Vector point) const {
  auto contains_point = [&](const Shape* shape) {
    return shape->Contains(point);
  };
  return std::all_of(contents.begin(), contents.end(), contains_point);
}

const char* CSGIntersection::Name() const { return "CSG Intersection"; }
