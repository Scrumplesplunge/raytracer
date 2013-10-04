#include "TraceRes.h"

TraceRes::TraceRes() :
	mask(0)
{}

TraceRes::TraceRes(const Primitive *prim) :
	mask(TraceRes::PRIMITIVE),
	primitive(prim)
{}

