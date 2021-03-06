#include "PasteSymbolCaptureOP.h"
#include "Math2D.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "sprite_msg.h"
#include "panel_msg.h"

namespace ee
{

PasteSymbolCaptureOP::PasteSymbolCaptureOP(wxWindow* wnd, EditPanelImpl* stage, LibraryPanel* library, 
										   PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt)
	: PasteSymbolOP(wnd, stage, library)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
{
	m_last_pos.MakeInvalid();
}

bool PasteSymbolCaptureOP::OnMouseLeftDown(int x, int y)
{
	Symbol* symbol = m_library->GetSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);
		m_last_pos = m_pos;

		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
	}

	return false;
}

bool PasteSymbolCaptureOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_last_pos.IsValid())
	{
		sm::vec2 offset = m_cmpt->GetOffset();
		sm::vec2 newPos = m_last_pos + offset;
		if (Math2D::GetDistance(m_pos, newPos) < 
			Math2D::GetDistance(offset, sm::vec2(0, 0)))
		{
			m_bCaptured = true;
			m_pos = newPos;
		}
	}
	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool PasteSymbolCaptureOP::Clear()
{
	if (PasteSymbolOP::Clear()) return true;

	m_last_pos.MakeInvalid();

	return false;
}

}