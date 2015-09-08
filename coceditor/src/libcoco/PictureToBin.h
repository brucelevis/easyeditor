#ifndef _LIBCOCO_PICTURE_TO_BIN_H_
#define _LIBCOCO_PICTURE_TO_BIN_H_

#include "PackPicture.h"

namespace libcoco
{

class PictureToBin
{
public:
	static int Size(const PackPicture* pic);

	static void Pack(const PackPicture* pic, uint8_t** ptr,
		const d2d::TexturePacker& tp);

private:
	static int SizeQuad();

	static void PackQuad(const PackPicture::Quad& quad, uint8_t** ptr, 
		const d2d::TexturePacker& tp);

}; // PictureToBin

}

#endif // _LIBCOCO_PICTURE_TO_BIN_H_