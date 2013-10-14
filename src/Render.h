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
		Vector (*renderPixel)(Ray);

		unsigned int
			currentChunk,
			numChunks;
		Image output;

		Camera* cam;
		Shape* shape;

		std::mutex mtx;
	public:
		unsigned int
			subPixelsX,
			subPixelsY,
			numThreads,
			chunkWidth,
			chunkHeight,
			baseMask;
	public:
		Render(Vector (*)(Ray));

		static void RenderChunk(Render*);
		
		Image operator()(Camera*, Shape*);
};

