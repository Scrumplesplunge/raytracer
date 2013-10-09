#pragma once
#include "Config.h"
#include "Matrix.h"
#include "Ray.h"

class Camera {
	private:
		Matrix transform;
		real width, height, mul;
	public:
		Camera(unsigned int, unsigned int, real);

		void moveTo(const Vector&);
		void lookAt(const Vector&);		

		Ray getRay(real, real, unsigned int);
};

