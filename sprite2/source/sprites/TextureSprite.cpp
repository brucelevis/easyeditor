#include "TextureSprite.h"

#include <stddef.h>

namespace s2
{

TextureSprite::TextureSprite(void* ud)
	: Sprite(ud)
{
}

TextureSprite::TextureSprite(const TextureSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}