#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/Camera.h>
#include <ee/color_config.h>
#include <ee/CameraMgr.h>

namespace etexpacker
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage_panel(stage)
	, m_stage_impl(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	sm::rect sr = m_screen.GetRegion();
	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage_impl->TraverseSprites(ee::DrawSpritesVisitor(sr, scale), ee::DT_VISIBLE);

	DrawRegion();
}

void StageCanvas::DrawRegion() const
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	// bg
	ee::RVG::Color(ee::LIGHT_RED);
	ee::RVG::Rect(sm::vec2(0, 0), sm::vec2(width, height), false);

	int x = 0, y = 0;
	int count = m_stage_panel->GetTextureAccount();
	ee::RVG::Color(ee::LIGHT_GREY);
	for (int i = 0; i < count; ++i)
	{
		ee::RVG::Rect(sm::vec2(x, y), sm::vec2(x + width, y + height), false);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}

	// physics
	m_stage_panel->drawPhysics();
}

}