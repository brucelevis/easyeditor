#include "StageCanvas.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/color_config.h>
#include <ee/cfg_const.h>

#include <easyscale9.h>

#include <sprite2/RenderParams.h>

namespace escale9
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
	, m_toolbar(NULL)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	if (m_toolbar->isComposeOP())
	{
		ComposeGrids::Draw();
		sm::rect sr = m_screen.GetRegion();
		float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
		m_stage_panel->TraverseSprites(ee::DrawSpritesVisitor(sr, scale), ee::DT_VISIBLE);
	}
	else
	{
		ee::RVG::Color(ee::LIGHT_GREY);
		ee::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);

		ee::Symbol* symbol = m_stage_panel->getPatchSymbol();
		if (symbol)
			symbol->Draw(s2::RenderParams());
	}
	m_stage_panel->DrawEditOP();
}

} // escale9