#include "SelectSpritesOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"
#include "GroupHelper.h"

#include <ee/EditPanelImpl.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/sprite_msg.h>
#include <ee/CrossGuides.h>
#include <ee/panel_msg.h>

namespace ecomplex
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(wnd, stage, sprites_impl, callback)
	, m_open_symbol(wnd, stage, sprites_impl)
	, m_guides(NULL)
{
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) {
		return true;
	}

	if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'G')
	{
		GroupSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'B')
	{
		BreakUpSelection();
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (m_guides && m_guides->OnMouseDown(m_stage->TransPosScrToProj(x, y))) {
		return true;
	} else {
		return ee::SelectSpritesOP::OnMouseLeftDown(x, y);
	}
}

bool SelectSpritesOP::OnMouseLeftUp(int x, int y)
{
	if (m_guides && m_guides->OnMouseUp(m_stage->TransPosScrToProj(x, y))) {
		return true;
	} else {
		return ee::SelectSpritesOP::OnMouseLeftUp(x, y);
	}
}

bool SelectSpritesOP::OnMouseDrag(int x, int y)
{
	if (m_guides && m_guides->OnMouseDrag(m_stage->TransPosScrToProj(x, y))) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	} else {
		return ee::SelectSpritesOP::OnMouseDrag(x, y);
	}
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (selected) {
		m_open_symbol.Open(selected, m_guides);
	}

	return false;
}

bool SelectSpritesOP::OnDraw() const
{
	bool ret = ee::SelectSpritesOP::OnDraw();
	if (m_guides) {
		m_guides->Draw();
	}
	return ret;

}

void SelectSpritesOP::GroupSelection()
{
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	const Symbol* parent = stage->GetSymbol();
	if (!parent || m_selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

	Sprite* spr = GroupHelper::Group(sprites);
	std::string filepath = ee::FileHelper::GetFileDir(parent->GetFilepath());
	filepath += "\\_tmp_";
	filepath += ee::StringHelper::ToString(wxDateTime::Now().GetTicks());
	filepath += "_" + ee::FileType::GetTag(ee::FileType::e_complex) + ".json";
	const_cast<Symbol&>(spr->GetSymbol()).SetFilepath(filepath);

	ee::InsertSpriteSJ::Instance()->Insert(spr);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::RemoveSpriteSJ::Instance()->Remove(sprites[i]);
	}
}

void SelectSpritesOP::BreakUpSelection()
{
	if (m_selection->IsEmpty()) {
		return;
	}

	std::string tag = "_" + ee::FileType::GetTag(ee::FileType::e_complex) + ".json";
	std::vector<ee::Sprite*> sprites;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprites[i];
		if (spr->GetSymbol().GetFilepath().find(tag) == std::string::npos) {
			continue;
		}
		
		ee::SelectSpriteSJ::Instance()->Select(spr, true);

		std::vector<ee::Sprite*> children;
		GroupHelper::BreakUp(spr, children);
		for (int j = 0, m = children.size(); j < m; ++j) {
			ee::Sprite* spr = children[j];
			ee::InsertSpriteSJ::Instance()->Insert(spr);
			spr->Release();
		}
		
		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		spr->Release();
	}
}

} // complex