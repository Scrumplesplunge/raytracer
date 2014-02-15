#include "Render.h"
#include "Ray.h"
#include "Random.h"

std::uniform_real_distribution<real> real_rand(0, 1);

Render::Render(Vector (*renderPixelFunc)(Shape*, Ray), Shape *s, Camera c) :
	renderPixel(renderPixelFunc),
	currentChunk(0),
	numChunks(0),
	numPasses(0),
	buffer(new Vector[c.getWidth() * c.getHeight()]),
	output(c.getWidth(), c.getHeight()),
	cam(c),
	shape(s),
	subPixels(1),
	numThreads(1),
	chunkWidth(64),
	chunkHeight(64),
	baseMask(TraceRes::DISTANCE | TraceRes::ENTERING | TraceRes::NORMAL | TraceRes::POSITION),
	brightness(0),
	contrast(1)
{}

Render::~Render() {
	delete[] buffer;
}

Image Render::operator()() {
	numChunks = ((cam.getWidth() + chunkWidth - 1) / chunkWidth) * ((cam.getHeight() + chunkHeight - 1) / chunkHeight);
	currentChunk = 0;
	numPasses++;
	
	std::thread threads[numThreads];
	for (unsigned int i = 0; i < numThreads; i++) threads[i] = std::thread(RenderChunk, this);
	for (unsigned int i = 0; i < numThreads; i++) threads[i].join();

	return output;
}

void Render::RenderChunk(Render* parent) {
	while (1) {
		parent->mtx.lock();
		// All chunks are taken.  Exit.
		if (parent->currentChunk == parent->numChunks) {
			parent->mtx.unlock();
			return;
		}
		unsigned int chunk = parent->currentChunk;
		parent->currentChunk++;
		parent->mtx.unlock();

		// Calculate the chunk coordinates.
		unsigned int chunksPerRow = (parent->cam.getWidth() + parent->chunkWidth - 1) / parent->chunkWidth;
		unsigned int x = parent->chunkWidth * (chunk % chunksPerRow);
		unsigned int y = parent->chunkHeight * (chunk / chunksPerRow);
		unsigned int x2 = (x + parent->chunkWidth <= parent->cam.getWidth()) ? x + parent->chunkWidth : parent->cam.getWidth();
		unsigned int y2 = (y + parent->chunkHeight <= parent->cam.getHeight()) ? y + parent->chunkHeight : parent->cam.getHeight();

		Image output(x2 - x, y2 - y);

		for (unsigned int yi = y; yi < y2; yi++) {
			for (unsigned int xi = x; xi < x2; xi++) {
				Vector color;

				// Supersampling.
                if (parent->subPixels == 0) {
                    color = parent->renderPixel(parent->shape, parent->cam.getRay(xi + 0.5, yi + 0.5, parent->baseMask));
                } else {
			        for (unsigned int i = 0, im = parent->subPixels; i < im; i++) {
				    	// Create a ray and trace it.
				    	Ray ray(parent->cam.getRay(xi + real_rand(random_generator), yi + real_rand(random_generator), parent->baseMask));
				        color = color + parent->renderPixel(parent->shape, ray);
				    }

				    // Take the average of all samples.
				    color = color / parent->subPixels;
                }

				Vector *ptr = parent->buffer + parent->cam.getWidth() * yi + xi;

				// Store the sum of all colours; used for calculating fine average over time.
				ptr[0] = ptr[0] + color;

				// Output the current average to the pixel.
				color = 256 * (parent->contrast * (ptr[0] / parent->numPasses) + parent->brightness);
				unsigned char *pix = output(xi - x, yi - y);
				pix[2] = (unsigned char)(color.x >= 256 ? 255 : (color.x < 0 ? 0 : color.x));
				pix[1] = (unsigned char)(color.y >= 256 ? 255 : (color.y < 0 ? 0 : color.y));
				pix[0] = (unsigned char)(color.z >= 256 ? 255 : (color.z < 0 ? 0 : color.z));
			}
		}

		parent->mtx.lock();
		parent->output.draw(output, x, y);
		parent->mtx.unlock();
	}
}

