#include "ArrangeSpriteImpl.h"
#include "Math2D.h"
#include "Config.h"
#include "SettingData.h"
#include "EditPanelImpl.h"
#include "MultiSpritesImpl.h"
#include "PropertySettingPanel.h"
#include "Camera.h"
#include "SpriteSelection.h"
#include "DeleteSpriteAOP.h"
#include "CombineAOP.h"
#include "TranslateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "ShearSpriteAOP.h"
#include "EE_RVG.h"
#include "panel_msg.h"
#include "RGBColorSettingDlg.h"
#include "HSLColorSettingDlg.h"
#include "AlphaSettingDlg.h"
#include "FetchAllVisitor.h"
#include "sprite_msg.h"
#include "EditOP.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "ScaleSpriteState.h"
#include "ShearSpriteState.h"
#include "OffsetSpriteState.h"
#include "MoveSpriteState.h"
#include "CopyPasteSpriteState.h"
#include "PerspectiveSpriteState.h"

#include <sprite2/RenderColor.h>

namespace ee
{

const float ArrangeSpriteImpl::CTRL_NODE_RADIUS = 10.0f;
const float ArrangeSpriteImpl::MAX_CTRL_NODE_RADIUS = 10.0f;

ArrangeSpriteImpl::ArrangeSpriteImpl(wxWindow* wnd, EditPanelImpl* stage,
									 MultiSpritesImpl* sprites_impl,
									 PropertySettingPanel* property,
									 const ArrangeSpriteConfig& cfg) 
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_property_panel(property)
	, m_align(sprites_impl)
	, m_op_state(NULL)
	, m_cfg(cfg)
	, m_popup(wnd, stage, sprites_impl, sprites_impl->GetSpriteSelection())
{
	m_align.SetOpen(cfg.is_auto_align_open);

	m_selection = sprites_impl->GetSpriteSelection();
	m_selection->Retain();

	m_left_down_pos.MakeInvalid();
	m_right_down_pos.MakeInvalid();
	
	m_ctrl_node_radius = CTRL_NODE_RADIUS;
}

ArrangeSpriteImpl::~ArrangeSpriteImpl()
{
	m_selection->Release();
	delete m_op_state;
}

bool ArrangeSpriteImpl::OnKeyDown(int keycode)
{
	if (m_stage->GetKeyState(WXK_SHIFT) && OnSpriteShortcutKey(keycode)) {
		return true;
	}

	bool ret = false;
	switch (keycode)
	{
	case WXK_DELETE:
		ret = true;
		OnDeleteKeyDown();
		break;
	case WXK_PAGEUP:
		ret = true;
		UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		ret = true;
		DownOneLayer();
		break;
	case '[':
		ret = true;
		DownOneLayer();
		break;
	case ']':
		ret = true;
		UpOneLayer();
		break;
	case 'm' : case 'M':
		ret = true;
		ChangeOPState(new MoveSpriteState(m_selection));
		break;
	case WXK_SPACE:
		ret = true;
		OnSpaceKeyDown();
		break;

// 		// for debug
// 	case 'O':
// 		ShaderMgr::Instance()->SetBufferData(true);
// 		break;
// 	case 'C':
// 		ShaderMgr::Instance()->SetBufferData(false);
// 		break;
	}

	return ret;
}

void ArrangeSpriteImpl::OnKeyUp(int keyCode)
{
	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::OnChar(int keycode)
{
	switch (keycode)
	{
	case 'a':
		OnDirectionKeyDown(KEY_LEFT);
		break;
	case 'd':
		OnDirectionKeyDown(KEY_RIGHT);
		break;
	case 's':
		OnDirectionKeyDown(KEY_DOWN);
		break;
	case 'w':
		OnDirectionKeyDown(KEY_UP);
		break;
	case 'A':
		OnDirectionKeyDown(KEY_LEFT | KEY_SHIFT);
		break;
	case 'D':
		OnDirectionKeyDown(KEY_RIGHT | KEY_SHIFT);
		break;
	case 'S':
		OnDirectionKeyDown(KEY_DOWN | KEY_SHIFT);
		break;
	case 'W':
		OnDirectionKeyDown(KEY_UP | KEY_SHIFT);
		break;
	}
}

void ArrangeSpriteImpl::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	m_left_down_pos = pos;

	m_align.SetInvisible();

	if (m_selection->IsEmpty()) {
		if (m_op_state) {
			m_op_state->OnMousePress(pos);
		}
		return;
	}

	// copy & paste
	if (m_stage->GetKeyState(WXK_ALT))
	{
		ChangeOPState(new CopyPasteSpriteState(m_selection));
	}

	Sprite* selected = NULL;
	if (m_selection->Size() == 1)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) {
		if (m_op_state) {
			m_op_state->OnMousePress(pos);
		}
		return;
	}

	// offset
	if (m_cfg.is_offset_open)
	{
		sm::vec2 offset = GetSprOffset(selected);
		if (Math2D::GetDistance(offset, pos) < m_ctrl_node_radius) {
			ChangeOPState(CreateOffsetState(selected));
			return;
		}
	}

	// scale
	if (m_cfg.is_deform_open && !m_stage->GetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math2D::GetDistance(ctrlNodes[i], pos) < m_ctrl_node_radius)
			{
				SpriteCtrlNode::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = SpriteCtrlNode::Type(i);
				ChangeOPState(CreateScaleState(selected, cn));
				return;
			}
		}
	}

	// perspective
	if (m_cfg.is_deform_open && m_stage->GetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrl_node[4];
		SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_node);
		for (int i = 0; i < 4; ++i) {
			if (Math2D::GetDistance(ctrl_node[i], pos) < m_ctrl_node_radius) {
				SpriteCtrlNode::Node cn;
				cn.pos = ctrl_node[i];
				cn.type = SpriteCtrlNode::Type(i);
				ChangeOPState(CreatePerspectiveState(selected, cn));
				return;
			}
		}
	}

	// translate
	ChangeOPState(CreateTranslateState(m_selection, pos));

	m_op_state->OnMousePress(pos);
}

void ArrangeSpriteImpl::OnMouseLeftUp(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state) 
	{
		m_op_state->OnMouseRelease(pos);
		ChangeOPState(NULL);
	}

	if (!m_selection->IsEmpty()) {
		sm::vec2 p;
		ChangeOPState(CreateTranslateState(m_selection, p));
	}

	if (m_cfg.is_auto_align_open &&
		Config::Instance()->GetSettings().open_sprite_capture
		&& m_align.IsOpen() 
		&& !m_selection->IsEmpty()
		&& m_left_down_pos != pos)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		m_align.Align(sprites);
		SetCanvasDirtySJ::Instance()->SetDirty();
	}

	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}

	m_left_down_pos.MakeInvalid();
}

void ArrangeSpriteImpl::OnMouseRightDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	m_right_down_pos = pos;

	Sprite* selected = NULL;
	if (m_selection->Size() == 1)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return;

	// shear
	if (m_cfg.is_deform_open)
	{
		sm::vec2 ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math2D::GetDistance(ctrlNodes[i], pos) < m_ctrl_node_radius)
			{
				SpriteCtrlNode::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = SpriteCtrlNode::Type(i);
				ChangeOPState(CreateShearState(selected, cn));
				return;
			}
		}
	}

	// rotate
	if (m_cfg.is_rotate_open) {
		ChangeOPState(CreateRotateState(m_selection, pos));
	}
}

void ArrangeSpriteImpl::OnMouseRightUp(int x, int y)
{
	if (!m_right_down_pos.IsValid()) {
		return;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (pos == m_right_down_pos)
	{
		wxMenu menu;
		SetRightPopupMenu(menu, x, y);
		m_stage->PopupMenu(&menu, x, y);
	}
	else if (m_op_state)
	{
		m_op_state->OnMouseRelease(pos);
		ChangeOPState(NULL);
	}

	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}

	m_right_down_pos.MakeInvalid();
}

void ArrangeSpriteImpl::OnMouseMove(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseMove(pos))
	{
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnMouseDrag(int x, int y)
{
// 	if (!m_left_down_pos.IsValid() && !m_right_down_pos.IsValid()) {
// 		return;
// 	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseDrag(pos))
	{
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnPopMenuSelected(int type)
{
	m_popup.OnRightPopupMenu(type);
}

void ArrangeSpriteImpl::OnDraw(const Camera& cam) const
{
	m_ctrl_node_radius = CTRL_NODE_RADIUS * cam.GetScale();
	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		Sprite* selected = NULL;
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];

		sm::vec2 sz = selected->GetRect().Size();
		float max_e = std::max(sz.x, sz.y);
		if (max_e / cam.GetScale() < 100) {
			m_ctrl_node_radius = 0;
		} else if (m_ctrl_node_radius > max_e * 0.1f) {
			m_ctrl_node_radius = 0;
		} else {
			if (m_stage->GetKeyState(WXK_SHIFT)) 
			{
				sm::vec2 ctrl_nodes[4];
				SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_nodes);
				for (int i = 0; i < 4; ++i) {
					RVG::Color(s2::Color(51, 204, 51));
					RVG::Circle(ctrl_nodes[i], m_ctrl_node_radius, true);
				}
			}
			else
			{
				sm::vec2 ctrl_nodes[8];
				SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrl_nodes);
				for (int i = 0; i < 4; ++i) {
					RVG::Color(s2::Color(51, 204, 51));
					RVG::Circle(ctrl_nodes[i], m_ctrl_node_radius, false);
				}
				for (int i = 4; i < 8; ++i) {
					RVG::Color(s2::Color(51, 204, 51));
					RVG::Circle(ctrl_nodes[i], m_ctrl_node_radius, true);
				}
			}

			if (m_cfg.is_offset_open)
			{
				sm::vec2 offset = GetSprOffset(selected);
				RVG::Color(s2::Color(204, 51, 51));
				RVG::Circle(offset, m_ctrl_node_radius, true);
			}
		}
	}

	m_align.Draw();
}

void ArrangeSpriteImpl::Clear()
{
}

Sprite* ArrangeSpriteImpl::QueryEditedSprite(const sm::vec2& pos) const
{
	Sprite* selected = NULL;
	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		std::vector<Sprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return NULL;

	if (m_cfg.is_offset_open)
	{
		sm::vec2 offset = GetSprOffset(selected);
		if (Math2D::GetDistance(offset, pos) < m_ctrl_node_radius) {
			return selected;
		}
	}

	if (m_cfg.is_deform_open && !m_stage->GetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrl_nodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrl_nodes);
		for (int i = 0; i < 8; ++i) {
			if (Math2D::GetDistance(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	if (m_cfg.is_deform_open && m_stage->GetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrl_nodes[4];
		SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_nodes);
		for (int i = 0; i < 4; ++i) {
			if (Math2D::GetDistance(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	return NULL;
}

bool ArrangeSpriteImpl::IsSelectionEmpty() const
{
	return m_selection->IsEmpty();
}

void ArrangeSpriteImpl::OnDirectionKeyDown(int type)
{
	if (!m_op_state) return;

	bool dirty = m_op_state->OnDirectionKeyDown(type);
	if (dirty)
	{
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnSpaceKeyDown()
{
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	if (sprites.empty()) {
		return;
	}

	CombineAOP* comb = new CombineAOP();
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		Sprite* sprite = sprites[i];

		comb->Insert(new TranslateSpriteAOP(sprite, -sprite->GetPosition()));
		comb->Insert(new ScaleSpriteAOP(sprite, sm::vec2(1, 1), sprite->GetScale()));
		comb->Insert(new ShearSpriteAOP(sprite, sm::vec2(0, 0), sprite->GetShear()));
		//comb->Insert(new OffsetSpriteAOP(sprite, sm::vec2(0, 0), sprite->getOffset()));

		sprite->SetTransform(sm::vec2(0, 0), 0);
		sprite->SetScale(sm::vec2(1, 1));
		sprite->SetShear(sm::vec2(0, 0));
		//sprite->setOffset(sm::vec2(0, 0));
	}
	EditAddRecordSJ::Instance()->Add(comb);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	m_popup.SetRightPopupMenu(menu, x, y);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateTranslateState(SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateRotateState(SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new RotateSpriteState(selection, first_pos);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateScaleState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new ScaleSpriteState(sprite, ctrl_node);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateShearState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new ShearSpriteState(sprite, ctrl_node);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreateOffsetState(Sprite* sprite) const
{
	return new OffsetSpriteState(sprite);
}

ArrangeSpriteState* ArrangeSpriteImpl::CreatePerspectiveState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new PerspectiveSpriteState(sprite, ctrl_node);
}

void ArrangeSpriteImpl::ChangeOPState(ArrangeSpriteState* state)
{
	if (m_op_state) {
		delete m_op_state;
	}
	m_op_state = state;
}

void ArrangeSpriteImpl::OnDeleteKeyDown()
{
	// add to history
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	EditAddRecordSJ::Instance()->Add(new DeleteSpriteAOP(sprites));

	m_sprites_impl->ClearSelectedSprite();

	ClearSpriteSelectionSJ::Instance()->Clear();
}

void ArrangeSpriteImpl::UpOneLayer()
{
	std::vector<Sprite*> selected;
	m_selection->Traverse(FetchAllVisitor<Sprite>(selected));

	SortSpriteSJ::Instance()->Sort(selected);
	for (int i = selected.size() - 1; i >= 0; --i) {
		ReorderSpriteSJ::Instance()->Reorder(selected[i], true);
	}

	ClearSpriteSelectionSJ::Instance()->Clear();
	for (int i = selected.size() - 1; i >= 0; --i) {
		SelectSpriteSJ::Instance()->Select(selected[i], false);
	}
}

void ArrangeSpriteImpl::DownOneLayer()
{
	std::vector<Sprite*> selected;
	m_selection->Traverse(FetchAllVisitor<Sprite>(selected));

	SortSpriteSJ::Instance()->Sort(selected);
	for (int i = 0, n = selected.size(); i < n; ++i) {
		ReorderSpriteSJ::Instance()->Reorder(selected[i], false);
	}

	ClearSpriteSelectionSJ::Instance()->Clear();
	for (int i = 0, n = selected.size(); i < n; ++i) {
		SelectSpriteSJ::Instance()->Select(selected[i], false);
	}
}

sm::vec2 ArrangeSpriteImpl::GetSprOffset(const Sprite* spr) const
{
	sm::vec2 offset = spr->GetPosition() + spr->GetOffset();
	return offset;
}

bool ArrangeSpriteImpl::OnSpriteShortcutKey(int keycode)
{
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	if (sprites.empty()) {
		return false;
	}

	sm::vec2 proj_pos = sprites[0]->GetPosition();
	sm::vec2 screen_pos = m_stage->TransPosProjToScr(proj_pos);
	wxPoint pos(screen_pos.x, screen_pos.y);

	// editable
	if (keycode == 'E')
	{
		bool editable = !sprites[0]->IsEditable();
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			ee::Sprite* spr = sprites[i];
			spr->SetEditable(editable);
		}
		RefreshPanelSJ::Instance()->Refresh();
		return true;
	}
	// visible
	else if (keycode == 'S')
	{
		bool visible = !sprites[0]->IsVisible();
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			ee::Sprite* spr = sprites[i];
			spr->SetVisible(visible);
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
		RefreshPanelSJ::Instance()->Refresh();
		return true;
	}
	// hori mirror
	else if (keycode == 'H') 
	{
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			Sprite* spr = sprites[i];
			spr->SetMirror(!spr->GetMirror().x, spr->GetMirror().y);
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	} 
	// vert mirror
	else if (keycode == 'V') 
	{
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			Sprite* spr = sprites[i];
			spr->SetMirror(spr->GetMirror().x, !spr->GetMirror().y);
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	} 
	// mul color
	else if (keycode == 'M') 
	{
		if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_RGB) {
			RGBColorSettingDlg dlg(m_wnd, NULL, sprites[0]->GetColor().mul, pos);
			if (dlg.ShowModal() == wxID_OK) {
				s2::Color col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->GetColor().mul = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		} else {
			HSLColorSettingDlg dlg(m_wnd, NULL, sprites[0]->GetColor().mul, pos);
			if (dlg.ShowModal() == wxID_OK) {
				s2::Color col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->GetColor().mul = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		return true;
	} 
	// add color
	else if (keycode == 'A') 
	{
		if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_RGB) {
			RGBColorSettingDlg dlg(m_wnd, NULL, sprites[0]->GetColor().add, pos);
			if (dlg.ShowModal() == wxID_OK) {
				s2::Color col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->GetColor().add = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		} else {
			HSLColorSettingDlg dlg(m_wnd, NULL, sprites[0]->GetColor().add, pos);
			if (dlg.ShowModal() == wxID_OK) {
				s2::Color col = dlg.GetColor();
				for (int i = 0, n = sprites.size(); i < n; ++i) {
					sprites[i]->GetColor().add = col;
				}
				SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		return true;
	}
	// alpha
	else if (keycode == 'T') 
	{
		AlphaSettingDlg dlg(m_wnd, sprites[0]->GetColor().mul, pos);
		float old_alpha = sprites[0]->GetColor().mul.a;
		int state = dlg.ShowModal();
		if (state == wxID_OK) {
			float alpha = dlg.GetColor().a;
			for (int i = 0, n = sprites.size(); i < n; ++i) {
				sprites[i]->GetColor().mul.a = alpha;
			}
			SetCanvasDirtySJ::Instance()->SetDirty();
		} else if (state == wxID_CANCEL) {
			sprites[0]->GetColor().mul.a = old_alpha;
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		return true;
	}
	return false;
}

}