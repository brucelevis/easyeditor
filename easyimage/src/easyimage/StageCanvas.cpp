#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/SpriteRenderer.h>
#include <ee/EE_RVG.h>

namespace eimage
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	if (const ee::Sprite* s = m_stage->GetImage())
	{
		ee::SpriteRenderer::Draw(s);
	}

	ee::RVG::Color(s2::Color(255, 0, 0));
	ee::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}
	
}