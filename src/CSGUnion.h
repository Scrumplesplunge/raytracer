#pragma once
#include "CSG.h"
#include "TraceRes.h"
#include "Vector.h"
#include "Array.h"

class CSGUnion : public CSG {
	private:
		Array<const Shape*> contents;
	public:
		CSGUnion();

		void add(const Shape*);

		virtual Array<TraceRes> trace(const Ray&) const;
		virtual bool contains(const Vector&) const;
		virtual const char *name() const;
};

