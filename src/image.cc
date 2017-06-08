#include "image.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

const char *Image::formats[] = {"BMP"};

const char *Image::errorStrings[] = {
    "I/O Error: Could not open file, perhaps it is missing or busy?",  // 0
    "I/O Error: Could not read from file.",                            // 1
    "Format Error: File format does not match the expected format.",   // 2
    "Format Error: Value out of range in file.",                       // 3
    "Format Error: Unsupported internal format.",                      // 4
    "Format Error: Unsupported file extension.",                       // 5
    "Runtime Error: Programmer did a silly.",                          // 6
    "I/O Error: Could not write to file."                              // 7
};

void Image::error(unsigned int errorID, const char *detailedMessage) {
  errorString = errorStrings[errorID];
  detailedErrorString = detailedMessage;
}

void Image::loadBMP(const char *filename) {
  // Try to open the image file.
  std::ifstream file(filename, std::ios::binary);

  // Exit if it failed to open.
  if (!file.good())
    return error(
        0, "The file stream did not have the good-bit set after opening.");

  // Read the file header.
  unsigned char tmp[32];
  file.read((char *)tmp, 14);

  // Exit if we failed to read the header.
  if (!file.good())
    return error(1,
                 "The file stream was not good after reading the file header "
                 "@(0x0, 14)");

  // Check that the file we are reading is, in fact, a bitmap.
  if (!(tmp[0] == 'B' && tmp[1] == 'M'))
    return error(1, "The first two bytes did not match 'BM'.");

  // Read the address that the image data starts at.
  unsigned int offset =
      ((unsigned int)tmp[10]) | (((unsigned int)tmp[11]) << 8) |
      (((unsigned int)tmp[12]) << 16) | (((unsigned int)tmp[13]) << 24);

  // Read the bitmap header size value.
  file.read((char *)tmp, 4);
  if (!file.good())
    return error(1,
                 "The file stream was not good after reading the DIB header "
                 "size @(0xE, 4)");
  unsigned int bitmapHeaderSize =
      ((unsigned int)tmp[0]) | (((unsigned int)tmp[1]) << 8) |
      (((unsigned int)tmp[2]) << 16) | (((unsigned int)tmp[3]) << 24);

  // Throw an error if the header size is something preposterous.
  if (bitmapHeaderSize > 128)
    return error(3,
                 "The specified bitmap header size exceeds 128, which (at the "
                 "time of writing) seems silly.");

  // I'm a combination of lazy and naive so I'm only supporting the 40-byte
  // BITMAPINFOHEADER.
  // What you gonna do - sue me?
  if (bitmapHeaderSize != 40)
    return error(4,
                 "This humble image loader only supports the 40-byte "
                 "BITMAPINFOHEADER for the DIB header.\nBe a lamb and convert "
                 "your image to use this format please.");

  // Load the image dimensions.
  file.read((char *)tmp, 8);
  if (!file.good())
    return error(1,
                 "The file stream was not good after reading the image "
                 "dimensions @(0x12, 8)");

  // The values are signed, to allow the rows to be stored bottom-to-top or
  // top-to-bottom etc.
  int signedWidth =
      (int)(((unsigned int)tmp[0]) | (((unsigned int)tmp[1]) << 8) |
            (((unsigned int)tmp[2]) << 16) | (((unsigned int)tmp[3]) << 24));
  int signedHeight =
      (int)(((unsigned int)tmp[4]) | (((unsigned int)tmp[5]) << 8) |
            (((unsigned int)tmp[6]) << 16) | (((unsigned int)tmp[7]) << 24));

  // Perform sanity checks on these values.
  if (signedWidth < 0)
    return error(3,
                 "Whilst negative heights have got a rational justification, "
                 "negative widths are just silly.");
  if (signedWidth == 0)
    return error(
        3, "There is literally no point in having a bitmap with a width of 0.");
  if (signedHeight == 0)
    return error(
        3,
        "There is literally no point in having a bitmap with a height of 0.");

  // Save the absolute width and height values.
  width = signedWidth;
  height = signedHeight < 0 ? -signedHeight : signedHeight;

  // Load various image configuration details.
  file.read((char *)tmp, 8);
  if (!file.good())
    return error(1,
                 "The file stream was not good after reading the configuration "
                 "details @(0x1A, 8)");

  if ((((unsigned int)tmp[0]) | (((unsigned int)tmp[1]) << 8)) != 1)
    return error(3,
                 "The number of colour planes is literally defined in the "
                 "specification as 1.");

  unsigned int bitsPerPixel =
      ((unsigned int)tmp[2]) | (((unsigned int)tmp[3]) << 8);
  if (bitsPerPixel != 24)
    return error(4,
                 "This humble image loader only supports 24-bit colour values "
                 "per pixel.");
  unsigned int compression =
      ((unsigned int)tmp[4]) | (((unsigned int)tmp[5]) << 8) |
      (((unsigned int)tmp[6]) << 16) | (((unsigned int)tmp[7]) << 24);
  if (compression != 0)
    return error(
        4, "This humble image loader does not support pixel compression.");

  // Load the image buffer size.
  file.read((char *)tmp, 4);
  if (!file.good())
    return error(1,
                 "The file stream was not good after reading the image buffer "
                 "size @(0x22, 4)");

  unsigned int dataSize =
      ((unsigned int)tmp[0]) | (((unsigned int)tmp[1]) << 8) |
      (((unsigned int)tmp[2]) << 16) | (((unsigned int)tmp[3]) << 24);

  // Quite a simple sanity check for the data size.
  if (dataSize < 3 * width * height)
    return error(
        3,
        "The image buffer size is smaller than the amount of required data.");

  // We don't actually care about the rest of the bitmap header for the purposes
  // of this image loader.
  file.seekg(offset);

  // Just load the image data, remove the padding, and put it in the image
  // buffer.
  unsigned char *imageTempBuffer = new unsigned char[dataSize];

  // Read the image data.
  file.read((char *)imageTempBuffer, dataSize);
  if (!file.good()) {
    delete[] imageTempBuffer;
    return error(1,
                 "The file stream was not good after reading the image data "
                 "@(<offset>, <dataSize>)");
  }

  // The imageBuffer will contain the image top-to-bottom, left-to-right.
  data = new unsigned char[3 * width * height];
  unsigned int srcRowSize = 4 * ((3 * width + 3) / 4);
  unsigned int destRowSize = 3 * width;
  unsigned int srcOffset = 0;
  unsigned int destOffset = 0;

  if (signedHeight < 0) {
    for (unsigned int i = 0; i < height; i++) {
      memcpy(data + destOffset, imageTempBuffer + srcOffset, destRowSize);
      srcOffset += srcRowSize;
      destOffset += destRowSize;
    }
  } else {
    srcOffset = height * srcRowSize;
    for (unsigned int i = 0; i < height; i++) {
      srcOffset -= srcRowSize;
      memcpy(data + destOffset, imageTempBuffer + srcOffset, destRowSize);
      destOffset += destRowSize;
    }
  }

  // Delete the temporary buffer.
  delete[] imageTempBuffer;

  // The image is loaded! Phew!
  success = true;
}

bool Image::saveBMP(const char *filename) {
  // Obviously, you can't write an image without an image to write.
  if (data == NULL) {
    error(6, "How exactly do you expect to save a NULL image?");
    return false;
  }

  // Open the output file.
  std::ofstream file(filename, std::ios::binary);

  // Die on failure.
  if (!file.good()) {
    error(0, "File was not good after attempting to open for writing.");
    return false;
  }

  // Necessary values.
  unsigned int srcRowWidth = 3 * width;
  unsigned int destRowWidth = 4 * ((3 * width + 3) / 4);
  unsigned int imageDataSize = destRowWidth * height;
  unsigned int fileSize = 54 + imageDataSize;

  // Write the image header.
  unsigned char header[54];
  header[0] = 'B', header[1] = 'M';

  // File size.
  header[2] = fileSize;
  header[3] = fileSize >> 8;
  header[4] = fileSize >> 16;
  header[5] = fileSize >> 24;

  // Offset.
  header[10] = 54;
  header[11] = 0;
  header[12] = 0;
  header[13] = 0;

  // DIB header starts here /////////////

  // DIB header size.
  header[14] = 40;
  header[15] = 0;
  header[16] = 0;
  header[17] = 0;

  // Image width.
  header[18] = width;
  header[19] = width >> 8;
  header[20] = width >> 16;
  header[21] = width >> 24;

  // Image height.
  header[22] = height;
  header[23] = height >> 8;
  header[24] = height >> 16;
  header[25] = height >> 24;

  // Number of colour planes := 1
  header[26] = 1;
  header[27] = 0;

  // Number of bits per pixel.
  header[28] = 24;
  header[29] = 0;

  // Compression method.
  header[30] = 0;
  header[31] = 0;
  header[32] = 0;
  header[33] = 0;

  // Image size.
  header[34] = imageDataSize;
  header[35] = imageDataSize >> 8;
  header[36] = imageDataSize >> 16;
  header[37] = imageDataSize >> 24;

  // Horizontal resolution - this value is just for giggles.
  header[38] = 41;
  header[39] = 25;
  header[40] = 0;
  header[41] = 0;

  // Vertical resolution - same story as before.
  header[42] = 57;
  header[43] = 5;
  header[44] = 0;
  header[45] = 0;

  // Number of colours in the colour palette.
  header[46] = 0;
  header[47] = 0;
  header[48] = 0;
  header[49] = 0;

  // Number of important colours used.
  header[50] = 0;
  header[51] = 0;
  header[52] = 0;
  header[53] = 0;

  // Write the header to the file.
  file.write((char *)header, 54);
  if (!file.good()) {
    error(7, "The file stream was not good after writing the image header.");
    return false;
  }

  // Write each row to the file.
  // Iterate backwards so that the rows are stored bottom-to-top.
  unsigned int srcOffset = height * srcRowWidth;
  char pad[3] = {
      'L', 'O',
      'L'};  // These are the padding bytes that fill in the end of each row.
  for (unsigned int i = 0; i < height; i++) {
    srcOffset -= srcRowWidth;
    file.write((char *)(data + srcOffset), srcRowWidth);
    file.write(pad, destRowWidth - srcRowWidth);
  }
  if (!file.good()) {
    error(7, "The file stream was not good after writing the image data.");
    return false;
  }

  // Image is written. Close and be done with it.
  file.close();
  return true;
}

void Image::cleanup() {
  if (data != NULL) {
    delete[] data;
  }

  success = false;
  errorString = NULL;
  detailedErrorString = NULL;
  width = height = 0;
  data = NULL;
}

void Image::clone(const Image &prototype) {
  cleanup();

  // These are easy to copy.
  success = prototype.success;
  errorString = prototype.errorString;
  detailedErrorString = prototype.detailedErrorString;
  width = prototype.width;
  height = prototype.height;

  // This is slightly more complex.
  if (prototype.data != NULL) {
    unsigned int bufferSize = 3 * width * height;
    data = new unsigned char[bufferSize];
    memcpy(data, prototype.data, bufferSize);
  }
}

Image::Image(unsigned int w, unsigned int h)
    : success(true),
      errorString(NULL),
      detailedErrorString(NULL),
      width(w),
      height(h),
      data(new unsigned char[width * height * 3]) {}

Image::Image(const char *filename)
    : success(false),
      errorString(NULL),
      detailedErrorString(NULL),
      width(0),
      height(0),
      data(NULL) {
  load(filename);
}

Image::Image(const Image &prototype)
    : success(false),
      errorString(NULL),
      detailedErrorString(NULL),
      width(0),
      height(0),
      data(NULL) {
  clone(prototype);
}

Image::~Image() { cleanup(); }

void Image::operator=(const Image &prototype) { clone(prototype); }

unsigned int Image::getWidth() const { return width; }

unsigned int Image::getHeight() const { return height; }

unsigned char *Image::operator()(unsigned int x, unsigned int y) const {
  if (x < 0 || width <= x || y < 0 || height <= y) return NULL;
  return data + 3 * (width * y + x);
}

int comp(const char *a, const char *b) {
  while (*a != 0 && *b != 0) {
    int diff = *a - *b;
    if (diff != 0) return diff;
    a++;
    b++;
  }
  // Termination happens when one (or both) of the strings is exhausted.
  if (*a == 0 && *b != 0) return -1;
  if (*a != 0 && *b == 0) return 1;
  return 0;
}

void Image::draw(const Image &source, unsigned int x, unsigned int y) {
  unsigned int maxX = x + source.getWidth();
  unsigned int maxY = y + source.getHeight();
  maxX = maxX < width ? maxX : width;
  maxY = maxY < height ? maxY : height;

  for (unsigned int yi = y; yi < maxY; yi++) {
    for (unsigned int xi = x; xi < maxX; xi++) {
      unsigned char *dst = operator()(xi, yi);
      unsigned char *src = source(xi - x, yi - y);
      dst[0] = src[0];
      dst[1] = src[1];
      dst[2] = src[2];
    }
  }
}

void Image::load(const char *filename) {
  cleanup();

  unsigned int length = strlen(filename);

  // Load bitmap.
  if (comp(filename + length - 4, ".bmp") == 0) return loadBMP(filename);

  // Format unsupported.
  return error(5,
               "Cannot load the specified file as its extension is "
               "unrecognised/unsupported.");
}

bool Image::save(const char *filename) {
  unsigned int length = strlen(filename);

  // Save bitmap.
  if (comp(filename + length - 4, ".bmp") == 0) return saveBMP(filename);

  // Format unsupported.
  error(5,
        "Cannot save to the specified file as its extension is "
        "unrecognised/unsupported.");
  return false;
}

bool Image::good() const { return success; }

void Image::printError() const {
  if (errorString == NULL) return;
  std::cerr << errorString << std::endl;
  if (detailedErrorString == NULL) return;
  std::cerr << detailedErrorString << std::endl;
}
