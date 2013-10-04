#pragma once
#include "Config.h"
#include "Vector.h"

class Primitive;

class TraceRes {
	public:
		unsigned int mask;
		const Primitive *primitive;
		Vector position, normal;
		real distance;
		bool entering;

		static const int
			PRIMITIVE	= 0x00000001,
			POSITION	= 0x00000002,
			NORMAL		= 0x00000004,
			DISTANCE	= 0x00000008,
			ENTERING	= 0x00000010;

		static int compare(TraceRes, TraceRes);
	public:
		TraceRes();
		TraceRes(const Primitive*);
};

