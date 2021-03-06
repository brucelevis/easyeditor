#ifndef _EASYEDITOR_MIRROR_SPRITE_AOP_H_
#define _EASYEDITOR_MIRROR_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class MirrorSpriteAOP : public AtomicOP
{
public:
	MirrorSpriteAOP(Sprite* sprite, bool old_mirror_x, bool old_mirror_y,
		bool new_mirror_x, bool new_mirror_y);
	virtual ~MirrorSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	Sprite* m_sprite;

	bool m_old_mirror_x, m_old_mirror_y;
	bool m_new_mirror_x, m_new_mirror_y;

}; // MirrorSpriteAOP 

}

#endif // _EASYEDITOR_MIRROR_SPRITE_AOP_H_