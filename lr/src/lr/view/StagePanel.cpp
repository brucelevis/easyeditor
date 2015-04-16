#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"

namespace lr
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_symbols_cfg(this, library)
	, m_viewlist(NULL)
{
	m_paste_op = new d2d::PasteSymbolOP(this, this, library);
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this);

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
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_viewlist->insert(sprite);
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	m_viewlist->reorder(sprite, up);
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

	if (m_editOP == m_arrange_op) {
		m_editOP = m_paste_op;
	} else {
		m_editOP = m_arrange_op;
	}
}

}