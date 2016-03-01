#ifndef _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_
#define _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace ee
{

class PerspectiveSpriteState : public ArrangeSpriteState
{
public:
	PerspectiveSpriteState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~PerspectiveSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

private:
	void Perspective(const Vector& curr);

private:
	Sprite* m_sprite;

	SpriteCtrlNode::Node m_ctrl_node;

	Vector m_first_persp;

}; // PerspectiveSpriteState

}

#endif // _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_