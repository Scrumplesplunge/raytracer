#include "csg_complement.h"
#include "config.h"

CSGComplement::CSGComplement(const Shape* shape) : contents(shape) {}

std::vector<TraceRes> CSGComplement::trace(const Ray& ray) const {
  Ray rayCopy(ray);

  // Required for CSG.
  rayCopy.mask |= TraceRes::ENTERING;

  // Perform the trace.
  std::vector<TraceRes> boundaries = contents->trace(ray);

  // Take its complement.
  for (TraceRes& boundary : boundaries) {
    // Invert this.
    boundary.entering = !boundary.entering;

    // If we have surface normals, invert these too.
    if (rayCopy.mask & TraceRes::NORMAL) boundary.normal = -boundary.normal;
  }

  // Return the result.
  return boundaries;
}

bool CSGComplement::contains(Vector vec) const {
  return !contents->contains(vec);
}

const char* CSGComplement::name() const { return "CSG Complement"; }
