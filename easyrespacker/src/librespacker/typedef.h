#ifndef _EASYRESPACKER_TYPEDEF_H_
#define _EASYRESPACKER_TYPEDEF_H_

#include <stdint.h>

namespace erespacker
{

static const float SCALE = 16;

static const uint8_t TAG_ID			= 1;
static const uint8_t TAG_COLOR		= 2;
static const uint8_t TAG_ADDITIVE	= 4;
static const uint8_t TAG_MATRIX		= 8;
static const uint8_t TAG_FILTER		= 16;
static const uint8_t TAG_CAMERA		= 32;
static const uint8_t TAG_COLMAP		= 64;
static const uint8_t TAG_BLEND		= 128;

enum TextureType
{
	TT_PNG4,
	TT_PNG8,
	TT_PVR,
	TT_ETC1,
	TT_ETC2,
};

}

#endif // _EASYRESPACKER_TYPEDEF_H_