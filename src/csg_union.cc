#include "csg_union.h"
#include "config.h"

#include <algorithm>
#include <iterator>

CSGUnion::CSGUnion() {}

void CSGUnion::add(const Shape* shape) { contents.push_back(shape); }

std::vector<TraceRes> CSGUnion::trace(const Ray& ray) const {
  Ray rayCopy(ray);

  // These are required for CSG.
  rayCopy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

  // Output array.
  std::vector<TraceRes> boundaries;

  // Try each branch of the union.
  bool inside = false;
  for (unsigned int i = 0; i < contents.size(); i++) {
    // Results from one branch, merged in with existing output.
    std::vector<TraceRes> branch_boundaries = contents[i]->trace(rayCopy);
    std::vector<TraceRes> merged_boundaries;
    std::merge(boundaries.begin(), boundaries.end(),
               branch_boundaries.begin(), branch_boundaries.end(),
               std::back_inserter(merged_boundaries));

    // Meh, this vaguely resembles a semaphore.
    unsigned int sem = 0;

    // Set the initial state of the semaphore.
    if (inside) sem++;
    bool isContained = contents[i]->contains(rayCopy.start);
    if (isContained) sem++;
    inside = inside || isContained;

    // Empty the output array, and generate the new one.
    boundaries.clear();
    for (const TraceRes& entry : merged_boundaries) {
      if (sem == 0) boundaries.push_back(entry);
      if (entry.entering) {
        sem++;
      } else {
        sem--;
      }
      if (sem == 0) boundaries.push_back(entry);
    }
  }

  // Return the result.
  return boundaries;
}

bool CSGUnion::contains(Vector vec) const {
  bool out = false;

  for (unsigned int i = 0; i < contents.size(); i++) {
    out = out || contents[i]->contains(vec);
  }

  return out;
}

const char* CSGUnion::name() const { return "CSG Union"; }
