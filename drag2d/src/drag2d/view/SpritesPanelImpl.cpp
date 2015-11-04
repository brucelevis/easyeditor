#include "SpritesPanelImpl.h"
#include "IDataContainer.h"
#include "EditPanelImpl.h"
#include "SpriteDropTarget.h"
#include "SpritesContainer.h"
#include "IStageCanvas.h"

#include "dataset/ISprite.h"
#include "message/subject_id.h"
#include "message/ReorderSpriteSJ.h"
#include "message/ReorderSpriteMostSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSJ.h"

namespace d2d
{

SpritesPanelImpl::SpritesPanelImpl(EditPanelImpl* stage, IDataContainer* container)
	: MultiSpritesImpl(stage)
	, m_stage(stage)
{
	m_container = container;
	m_container->Retain();

	m_subjects.push_back(ReorderSpriteSJ::Instance());
	m_subjects.push_back(ReorderSpriteMostSJ::Instance());
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

SpritesPanelImpl::SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library)
	: MultiSpritesImpl(stage)
	, m_stage(stage)
{
	m_stage->SetDropTarget(new SpriteDropTarget(this, stage, library));

	m_container = new SpritesContainer;

	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

SpritesPanelImpl::~SpritesPanelImpl()
{
	m_container->Release();
}

void SpritesPanelImpl::Notify(int sj_id, void* ud)
{
	MultiSpritesImpl::Notify(sj_id, ud);

	switch (sj_id)
	{
	case REORDER_SPRITE:
		{
			ReorderSpriteSJ::Params* p = (ReorderSpriteSJ::Params*)ud;
			m_container->ResetOrder(p->spr, p->up);
			m_stage->SetCanvasDirty();
		}
		break;
	case REORDER_SPRITE_MOST:
		{
			ReorderSpriteMostSJ::Params* p = (ReorderSpriteMostSJ::Params*)ud;
			m_container->ResetOrderMost(p->spr, p->up);
			m_stage->SetCanvasDirty();
		}
		break;
	case INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			m_container->Insert(p->spr, p->idx);
			m_stage->SetCanvasDirty();
		}
		break;
	case REMOVE_SPRITE:
		{
			m_container->Remove((ISprite*)ud);
			m_stage->SetCanvasDirty();
		}
		break;
	case CLEAR:
		{
			m_container->Clear();
			m_stage->SetCanvasDirty();
		}
		break;
	}
}

void SpritesPanelImpl::TraverseSprites(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/,
									   bool order/* = true*/) const
{
	m_container->Traverse(visitor, type, order);
}

}