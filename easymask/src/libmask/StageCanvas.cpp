#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/RenderParams.h>

namespace emask
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_mask_render(false)
{
}

void StageCanvas::OnDrawSprites() const
{
	Symbol* sym = m_stage->GetSymbol();
	if (m_mask_render) {
		sym->Draw(s2::RenderParams(), NULL);
	} else {
		const ee::Symbol *base = sym->GetSymbol(true),
			*mask = sym->GetSymbol(false);
		if (base) {
			ee::SpriteRenderer::Draw(base);
		}
		if (mask) {
			ee::SpriteRenderer::Draw(mask);
		}
	}

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif
}

}