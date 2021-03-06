#ifndef _GLUE_JPEG_LOADER_H_
#define _GLUE_JPEG_LOADER_H_

#include <string>
#include <stdint.h>

#include <render/render.h>

namespace glue
{

class JpegLoader
{
public:
	static void Write(const uint8_t* pixels, int width, int height, const char* filename, int quality);
	static uint8_t* Read(const char* filename, int& width, int& height, int& channels);

}; // JpegLoader

}

#endif // _GLUE_JPEG_LOADER_H_