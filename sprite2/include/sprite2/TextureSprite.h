#ifndef _SPRITE2_TEXTURE_SPRITE_H_
#define _SPRITE2_TEXTURE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class TextureSprite : public Sprite
{
public:
	TextureSprite(void* ud);
	TextureSprite(const TextureSprite& spr, void* ud);

	virtual bool Update(float dt) { return false; }
	
private:
	TextureSprite() {}
	const TextureSprite& operator = (const TextureSprite& spr) { return *this; }

}; // TextureSprite

}

#endif // _SPRITE2_TEXTURE_SPRITE_H_