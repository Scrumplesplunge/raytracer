#include "Render.h"
#include "Ray.h"

Render::Render(Vector (*renderPixelFunc)(Shape*, Ray)) :
	renderPixel(renderPixelFunc),
	currentChunk(0),
	numChunks(0),
	output(0, 0),
	subPixelsX(1),
	subPixelsY(1),
	numThreads(1),
	chunkWidth(32),
	chunkHeight(32),
	baseMask(TraceRes::DISTANCE | TraceRes::ENTERING | TraceRes::NORMAL)
{}

Image Render::operator()(Shape *shapePtr, Camera *camPtr) {
	output = Image(camPtr->getWidth(), camPtr->getHeight());

	cam = camPtr;
	shape = shapePtr;
	numChunks = ((camPtr->getWidth() + chunkWidth - 1) / chunkWidth) * ((camPtr->getHeight() + chunkHeight - 1) / chunkHeight);
	currentChunk = 0;
	
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
		unsigned int chunksPerRow = (parent->cam->getWidth() + parent->chunkWidth - 1) / parent->chunkWidth;
		unsigned int x = parent->chunkWidth * (chunk % chunksPerRow);
		unsigned int y = parent->chunkHeight * (chunk / chunksPerRow);
		unsigned int x2 = (x + parent->chunkWidth <= parent->cam->getWidth()) ? x + parent->chunkWidth : parent->cam->getWidth();
		unsigned int y2 = (y + parent->chunkHeight <= parent->cam->getHeight()) ? y + parent->chunkHeight : parent->cam->getHeight();

		Image buffer(x2 - x, y2 - y);

		for (unsigned int yi = y; yi < y2; yi++) {
			for (unsigned int xi = x; xi < x2; xi++) {
				Vector color;
				for (unsigned int aay = 0, aaymax = parent->subPixelsY; aay < aaymax; aay++) {
					for (unsigned int aax = 0, aaxmax = parent->subPixelsX; aax < aaxmax; aax++) {
						Ray ray(parent->cam->getRay(xi + (real(aax) + 0.5) / aaxmax, yi + (real(aay) + 0.5) / aaymax, parent->baseMask));
						color = color + parent->renderPixel(parent->shape, ray);
					}
				}
				color = color / (parent->subPixelsX * parent->subPixelsY);
				unsigned char *pix = buffer(xi - x, yi - y);
				pix[2] = (unsigned char)(255.9 * color.x);
				pix[1] = (unsigned char)(255.9 * color.y);
				pix[0] = (unsigned char)(255.9 * color.z);
			}
		}

		parent->mtx.lock();
		parent->output.draw(buffer, x, y);
		parent->mtx.unlock();
	}
}

