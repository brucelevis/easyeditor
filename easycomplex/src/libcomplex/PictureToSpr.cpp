#include "PictureToSpr.h"
#include "Symbol.h"
#include "Sprite.h"

#include <easyicon.h>

namespace ecomplex
{

d2d::ISprite* PictureToSpr::Trans(const libcoco::PackPicture* pic)
{
	if (pic->quads.size() == 1) {
		return TransQuad(&pic->quads[0]);
	} else if (pic->quads.size() > 1) {
		ecomplex::Symbol* complex = new ecomplex::Symbol;
		for (int i = 0, n = pic->quads.size(); i < n; ++i) {
			complex->m_sprites.push_back(TransQuad(&pic->quads[i]));
		}
		complex->InitBounding();
		return new Sprite(complex);
	} else {
		return NULL;
	}
}

d2d::ISprite* PictureToSpr::TransQuad(const libcoco::PackPicture::Quad* quad)
{
	float w = quad->img->GetOriginWidth(),
		h = quad->img->GetOriginHeight();

	d2d::Vector src[4], screen[4];
	for (int i = 0; i < 4; ++i) {
		src[i].x = quad->texture_coord[i].x / w;
		src[i].y = quad->texture_coord[i].y / h;
		screen[i] = quad->screen_coord[i];
	}

	eicon::QuadIcon* icon = new eicon::QuadIcon(
		const_cast<d2d::Image*>(quad->img), src, screen);

	eicon::Symbol* symbol = new eicon::Symbol;
	symbol->SetIcon(icon);
	d2d::ISprite* ret = new eicon::Sprite(symbol);
	ret->BuildBounding();
	return ret;
}

}