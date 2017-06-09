#include "csg_intersection.h"
#include "config.h"

#include <algorithm>
#include <iterator>

CSGIntersection::CSGIntersection() {}

void CSGIntersection::add(const Shape* shape) { contents.push_back(shape); }

std::vector<TraceRes> CSGIntersection::trace(const Ray& ray) const {
  Ray rayCopy(ray);

  // These are required for CSG.
  rayCopy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  if (contents.size() == 0) return {};

  std::vector<TraceRes> boundaries = contents[0]->trace(rayCopy);

  // Keep track of whether the start is inside the CSGIntersection shape.
  bool inside = contents[0]->contains(rayCopy.start);

  // If there are no intersections and we aren't inside, then there is no
  // intersection at all.
  if (boundaries.size() == 0 && !inside) return boundaries;

  for (unsigned int i = 1; i < contents.size(); i++) {
    // Results from one branch, merged in with existing output.
    std::vector<TraceRes> branch_boundaries = contents[i]->trace(rayCopy);
    bool isContained = contents[i]->contains(rayCopy.start);

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

bool CSGIntersection::contains(Vector vec) const {
  if (contents.size() == 0) return false;

  bool out = true;

  for (unsigned int i = 0; i < contents.size(); i++) {
    out = out && contents[i]->contains(vec);
  }

  return out;
}

const char* CSGIntersection::name() const { return "CSG Intersection"; }
