#pragma once
#include "Config.h"
#include "Matrix.h"
#include "Ray.h"

class Camera {
	private:
		Matrix transform;
		unsigned int width, height;
		real mul;
	public:
		Camera(unsigned int, unsigned int, real);

		void moveTo(const Vector&);
		void lookAt(const Vector&);

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		Ray getRay(real, real, unsigned int);
};

