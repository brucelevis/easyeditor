#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace eshadow
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
	, m_edited(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, d2d::ISprite* edited, 
						 const std::vector<d2d::ISprite*>& bg_sprites)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
	, m_edited(edited)
	, m_bg_sprites(bg_sprites)
{
}

void StageCanvas::initGL()
{
	d2d::DynamicStageCanvas::initGL();

	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	} else {
		m_stage->GetSymbol()->reloadTexture();
		for (int i = 0, n = m_bg_sprites.size(); i < n; ++i) {
			m_bg_sprites[i]->getSymbol().reloadTexture();
		}
	}

	resetViewport();
}

void StageCanvas::OnDraw()
{
	if (m_edited) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		for (int i = 0, n = m_bg_sprites.size(); i < n; ++i) {
			d2d::SpriteRenderer::Instance()->Draw(m_bg_sprites[i], mat);
		}
	}

	m_stage->GetSymbol()->draw(d2d::Matrix());
	m_stage->drawEditTemp();
}

}