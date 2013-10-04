#pragma once
#include "CSG.h"
#include "TraceRes.h"
#include "Ray.h"
#include "Vector.h"

class CSGComplement : public CSG {
	private:
		const Shape* contents;
	public:
		CSGComplement(const Shape*);

		virtual Array<TraceRes> trace(const Ray&) const;
		virtual bool contains(const Vector&) const;
		virtual const char *name() const;
};

