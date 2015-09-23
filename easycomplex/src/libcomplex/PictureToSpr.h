#ifndef _LIBCOMPLEX_PICTURE_TO_SPR_H_
#define _LIBCOMPLEX_PICTURE_TO_SPR_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class PictureToSpr
{
public:
	static d2d::ISprite* Trans(const libcoco::PackPicture* pic);

private:
	static d2d::ISprite* TransQuad(const libcoco::PackPicture::Quad* quad);

}; // PictureToSpr

}

#endif // _LIBCOMPLEX_PICTURE_TO_SPR_H_