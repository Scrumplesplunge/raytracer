#include "CSGIntersection.h"

CSGIntersection::CSGIntersection() {}

void CSGIntersection::add(const Shape* shape) {
	contents.push(shape);
}

Array<TraceRes> CSGIntersection::trace(const Ray& ray) const {
	Ray rayCopy(ray);

	// These are required for CSG.
	rayCopy.mask |= TraceRes::DISTANCE | TraceRes::ENTERING;

	// Output array.
	Array<TraceRes> out;
	if (contents.length() == 0) return out;
	out = contents[0]->trace(rayCopy);

	// Try each branch of the intersection.
	bool inside = contents[0]->contains(rayCopy.start);
	for (unsigned int i = 1; i < contents.length(); i++) {
		// Results from one branch, merged in with existing output.
		Array<TraceRes> branch(contents[i]->trace(rayCopy));
		Array<TraceRes> temp(out.merge(branch, TraceRes::compare));

		// Meh, this vaguely resembles a semaphore.
		unsigned int sem = 0;

		// Set the initial state of the semaphore.
		if (inside) sem++;
                bool isContained = contents[i]->contains(rayCopy.start);
                if (isContained) sem++;
                inside = inside && isContained;

		// Empty the output array, and generate the new one.
		out = Array<TraceRes>();
		for (unsigned int j = 0; j < temp.length(); j++) {
			if (sem == 2) out.push(temp[j]);
			if (temp[j].entering) {
				sem++;
			} else {
				sem--;
			}
			if (sem == 2) out.push(temp[j]);
		}
	}

	// Return the result.
	return out;
}

bool CSGIntersection::contains(const Vector& vec) const {
	if (contents.length() == 0) return false;

	bool out = true;
	
	for (unsigned int i = 0; i < contents.length(); i++) {
		out = out && contents[i]->contains(vec);
	}

	return out;
}

const char *CSGIntersection::name() const {
	return "CSG Intersection";
}

