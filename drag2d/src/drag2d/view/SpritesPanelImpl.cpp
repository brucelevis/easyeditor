#include "SpritesPanelImpl.h"
#include "IDataContainer.h"
#include "EditPanel.h"
#include "SpriteDropTarget.h"
#include "SpritesContainer.h"

namespace d2d
{

SpritesPanelImpl::SpritesPanelImpl(wxWindow* parent, IDataContainer* container)
	: MultiSpritesImpl(parent)
{
	m_container = container;
	m_container->Retain();
}

SpritesPanelImpl::SpritesPanelImpl(EditPanel* editPanel, LibraryPanel* libraryPanel)
	: MultiSpritesImpl(editPanel)
{
	editPanel->SetDropTarget(new SpriteDropTarget(this, editPanel, libraryPanel));

	m_container = new SpritesContainer;
}

SpritesPanelImpl::~SpritesPanelImpl()
{
	m_container->Release();
}

bool SpritesPanelImpl::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	return m_container->ResetOrder(sprite, up);
}

bool SpritesPanelImpl::InsertSprite(ISprite* sprite)
{
	m_container->Insert(sprite);
	return true;
}

bool SpritesPanelImpl::RemoveSprite(ISprite* sprite)
{
	return m_container->Remove(sprite);
}

bool SpritesPanelImpl::ClearAllSprite()
{
	return m_container->Clear();
}

void SpritesPanelImpl::TraverseSprites(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/,
									   bool order/* = true*/) const
{
	m_container->Traverse(visitor, type, order);
}

}