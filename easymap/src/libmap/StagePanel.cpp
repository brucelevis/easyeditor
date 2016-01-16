#include "StagePanel.h"
#include "StageCanvas.h"

namespace emap
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library,
					   d2d::PropertySettingPanel* property)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, d2d::ShapesPanelImpl(GetStageImpl())
{
	m_paste_op = new d2d::PasteSymbolOP(this, GetStageImpl(), this, library);
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property);

	SetEditOP(m_paste_op);
	SetCanvas(new StageCanvas(this));
}

StagePanel::~StagePanel()
{
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
	if (!GetEditOP()->IsEmpty()) {
		return;
	}

	if (GetEditOP() == m_arrange_op) {
		SetEditOP(m_paste_op);
	} else {
		SetEditOP(m_arrange_op);
	}
}

}