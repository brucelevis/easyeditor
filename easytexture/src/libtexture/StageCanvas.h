#ifndef _EASYTEXTURE_STAGE_CANVAS_H_
#define _EASYTEXTURE_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class Sprite; class MultiSpritesImpl; }

namespace etexture
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_panel;

	ee::Sprite* m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::Sprite* m_bg;

}; // StageCanvas

}

#endif // _EASYTEXTURE_STAGE_CANVAS_H_
