#ifndef _EASYEDITOR_PPM_ADAPTER_H_
#define _EASYEDITOR_PPM_ADAPTER_H_

#include <string>

#include <stdint.h>

namespace ee
{

class PPMAdapter
{
public:
	static int Write(const uint8_t* pixels, int width, int height,
		const std::string& filename);

	static uint8_t* Read(const std::string& filename, int& width, int& height);

private:
	static uint8_t* ReadPNM(const std::string& filename, int& width, int& height);
	static uint8_t* ReadPPM(const std::string& filename, int& width, int& height);
	static uint8_t* ReadPGM(const std::string& filename, int& width, int& height);

}; // PPMAdapter

}

#endif // _EASYEDITOR_PPM_ADAPTER_H_