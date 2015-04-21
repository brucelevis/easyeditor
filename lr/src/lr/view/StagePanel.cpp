#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"

#include "frame/config.h"

namespace lr
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::PropertySettingPanel* property,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_symbols_cfg(this, library)
	, m_viewlist(NULL)
	, m_sindex(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN))
	, m_pathfinding(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN), 256, 256)
{
	m_paste_op = new d2d::PasteSymbolOP(this, this, library);
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property);

	m_editOP = m_paste_op;
	m_editOP->Retain();

	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	m_paste_op->Release();
	m_arrange_op->Release();
}

void StagePanel::clear()
{
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	m_viewlist->remove(sprite);

	m_pathfinding.DisableRegion(sprite->GetRect(), true);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_viewlist->insert(sprite);
	m_sindex.Insert(sprite);

	m_pathfinding.DisableRegion(sprite->GetRect(), false);
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	m_viewlist->reorder(sprite, up);
}

void StagePanel::DebugDraw() const
{
	m_sindex.DebugDraw();
	m_pathfinding.DebugDraw();
}

void StagePanel::Pathfinding(const d2d::Vector& start, const d2d::Vector& end)
{
	m_pathfinding.QueryRoute(start, end);
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
	if (!m_editOP->IsEmpty()) {
		return;
	}

	m_editOP->Release();
	if (m_editOP == m_arrange_op) {
		m_editOP = m_paste_op;
	} else {
		m_editOP = m_arrange_op;
	}
	m_editOP->Retain();
}

}