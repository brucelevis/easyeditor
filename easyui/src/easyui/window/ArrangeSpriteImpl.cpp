#include "ArrangeSpriteImpl.h"
#include "TranslateSpriteState.h"
#include "StagePanel.h"
#include "MoveSpriteCenterState.h"

#include <ee/EE_RVG.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Camera.h>
#include <ee/color_config.h>
#include <ee/Math2D.h>
#include <ee/std_functor.h>
#include <ee/panel_msg.h>

#include <easyui.h>
#include <easycomplex.h>

namespace eui
{
namespace window
{

static const float CENTER_NODE_RADIUS		= 10.0f;
static const float MAX_CENTER_NODE_RADIUS	= 10.0f;

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
	, m_stage(stage)
	, m_center_node_radius(CENTER_NODE_RADIUS)
	, m_move_center(false)
	, m_selected(NULL)
{
}

void ArrangeSpriteImpl::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(m_selected);
	if (complex && ee::Math2D::GetDistance(pos, m_selected->GetPosition()) < m_center_node_radius) {
		m_move_center = true;
		ChangeOPState(new MoveSpriteCenterState(complex, pos));		
	} else {
		ee::ArrangeSpriteImpl::OnMouseLeftDown(x, y);
	}
}

void ArrangeSpriteImpl::OnMouseLeftUp(int x, int y)
{
	ee::ArrangeSpriteImpl::OnMouseLeftUp(x, y);

	m_move_center = false;
	m_selected = NULL;
}

void ArrangeSpriteImpl::OnDraw(const ee::Camera& cam) const
{
	ee::ArrangeSpriteImpl::OnDraw(cam);

	if (!m_move_center) {
		std::vector<ee::Sprite*> sprites;
		m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
		if (sprites.size() == 1) {
			m_selected = sprites[0];
		}
	}

	if (m_selected) {
		sm::vec2 pos = m_selected->GetPosition();
		m_center_node_radius = std::min(CENTER_NODE_RADIUS * cam.GetScale(), MAX_CENTER_NODE_RADIUS);
		ee::RVG::Color(ee::RED);
		ee::RVG::Circle(pos, m_center_node_radius, false);
		ee::RVG::Color(ee::GREEN);
		ee::RVG::Cross(pos, m_center_node_radius * 2, m_center_node_radius * 2);
	}
}

ee::ArrangeSpriteState* ArrangeSpriteImpl::
CreateTranslateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new TranslateSpriteState(m_stage, selection, first_pos, &m_stage->GetSymbol()->GetAnchorMgr()); 
}

}
}