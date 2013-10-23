#pragma once
#include "Array.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "Shape.h"
#include "Vector.h"
#include <thread>
#include <mutex>

class Render {
	private:
		Vector (*renderPixel)(Shape*, Ray);

		unsigned int
			currentChunk,
			numChunks,
			numPasses;

		Vector *buffer;
		Image output;

		Camera cam;
		Shape *shape;

		std::mutex mtx;
	public:
		unsigned int
			subPixelsX,
			subPixelsY,
			numThreads,
			chunkWidth,
			chunkHeight,
			baseMask;
		real brightness, contrast;
	public:
		Render(Vector (*)(Shape*, Ray), Shape*, Camera);
		~Render();

		static void RenderChunk(Render*);
		
		Image operator()();
};

