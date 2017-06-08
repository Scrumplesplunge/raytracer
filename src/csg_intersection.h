#pragma once

#include "csg.h"
#include "trace_res.h"
#include "vector.h"
#include "array.h"

class CSGIntersection : public CSG {
	private:
		Array<const Shape*> contents;
	public:
		CSGIntersection();

		void add(const Shape*);

		virtual Array<TraceRes> trace(const Ray&) const;
		virtual bool contains(const Vector&) const;
		virtual const char *name() const;

		virtual real minX() const;
		virtual real minY() const;
		virtual real minZ() const;

		virtual real maxX() const;
		virtual real maxY() const;
		virtual real maxZ() const;
};

