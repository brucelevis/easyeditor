#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/EE_RVG.h>

namespace eui
{
namespace list
{

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), glctx, false)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	ee::Rect sr = m_screen.GetRegion();
	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, scale), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	ee::RVG::Color(ee::Colorf(1, 0, 0));
	ee::RVG::Cross(ee::Vector(0, 0), 100, 100);
}

}
}