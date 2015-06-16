#include "StageCanvas.h"
#include "StagePanel.h"

namespace eimage
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::OnDraw()
{
	if (const d2d::ISprite* s = m_stage->getImage())
	{
		d2d::SpriteRenderer::Instance()->Draw(s);
	}

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	m_stage->drawEditTemp();
}
	
}