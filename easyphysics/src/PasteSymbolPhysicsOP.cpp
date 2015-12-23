#include "PasteSymbolPhysicsOP.h"
#include "PhysicsPanelImpl.h"
#include "IBody.h"
#include "BodyManager.h"

namespace ephysics
{

PasteSymbolPhysicsOP::PasteSymbolPhysicsOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
										   d2d::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, 
										   float* pScale /*= NULL*/)
	: d2d::PasteSymbolOP(stage_wnd, stage, libraryPanel, pScale)
	, m_pScale(pScale)
	, m_bStatic(false)
{
	m_world = physicsImpl->getWorld();	
}

bool PasteSymbolPhysicsOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	d2d::ISymbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Translate(m_pos);
		if (m_pScale) {
			sprite->SetScale(d2d::Vector(*m_pScale, *m_pScale));
		}

		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}
		d2d::InsertSpriteSJ::Instance()->Insert(sprite);

		sprite->Release();
	}

	return false;
}

}
