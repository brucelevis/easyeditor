#include "PasteSymbolCaptureOP.h"

#include "common/Math.h"
#include "dataset/SpriteFactory.h"
#include "dataset/ISprite.h"
#include "view/MultiSpritesImpl.h"
#include "view/LibraryPanel.h"
#include "view/IStageCanvas.h"

namespace d2d
{

PasteSymbolCaptureOP::PasteSymbolCaptureOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
										   LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
{
	m_lastPos.setInvalid();
}

bool PasteSymbolCaptureOP::OnMouseLeftDown(int x, int y)
{
	ISymbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);
		m_lastPos = m_pos;

		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->Translate(m_pos);
		m_panelImpl->InsertSprite(sprite);
		sprite->Release();
	}

	return false;
}

bool PasteSymbolCaptureOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_lastPos.isValid())
	{
		Vector offset = m_cmpt->getOffset();
		Vector newPos = m_lastPos + offset;
		if (Math::getDistance(m_pos, newPos) < 
			Math::getDistance(offset, Vector(0, 0)))
		{
			m_bCaptured = true;
			m_pos = newPos;
		}
	}
	m_stage->GetCanvas()->SetDirty();

	return false;
}

bool PasteSymbolCaptureOP::Clear()
{
	if (PasteSymbolOP::Clear()) return true;

	m_lastPos.setInvalid();

	return false;
}

} // d2d