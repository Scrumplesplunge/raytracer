#pragma once

class Image {
	public:
		enum Format {
			BMP,
			INVALID	// This doubles as an accessor to the number of valid states. Must be last.
		};

		static const char *formats[];

		static const char *errorStrings[];

		void error(unsigned int, const char*);
	private:
		bool success;
		const char *errorString;
		const char *detailedErrorString;

		unsigned int width, height;
		unsigned char *data;

		void loadBMP(const char*);

		bool saveBMP(const char*);

		void cleanup();
		void clone(const Image&);
	public:
		// Class construction / destruction.
		Image(unsigned int, unsigned int);
		Image(const char*);
		Image(const Image&);
		~Image();

		// Copy.
		void operator=(const Image&);

		// Pixel access.
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned char *operator()(unsigned int, unsigned int);

		// File I/O
		void load(const char*);
		bool save(const char*);
		bool good() const;
		void printError() const;
};

