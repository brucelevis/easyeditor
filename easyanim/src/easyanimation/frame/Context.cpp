#include "Context.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "view/KeysPanel.h"

namespace eanim
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	library = NULL;
	property = NULL;
	stage = NULL;
	toolbar = NULL;
	layersPanel = NULL;
	keysPanel = NULL;
	viewlist = NULL;

	m_curr_layer = 0;
	m_curr_frame = 1;
	maxFrame = -1;

	fps = 30;

	m_last_keyframe = NULL;
}

void Context::setCurrFrame(int layer, int frame)
{
	m_curr_layer = layer;
	m_curr_frame = frame;

	Layer* pLayer = layers.getLayer(layer);
	if (!pLayer) 
	{
		m_last_keyframe = NULL;
		return;
	}

	KeyFrame* pFrame = pLayer->getCurrKeyFrame(frame);
	if (!pFrame) 
	{
		m_last_keyframe = NULL;
		return;
	}

	if (pFrame != m_last_keyframe) 
	{
		reloadViewList(*pFrame);
		m_last_keyframe = pFrame;

		if (keysPanel) 
		{
			int row, col;
			keysPanel->getSelectPos(row, col);
			col = m_curr_frame - 1;
			keysPanel->setSelectPos(row, col);
		}

		stage->Refresh();
		if (keysPanel) {
			keysPanel->Refresh();
		}
		if (layersPanel) {
			layersPanel->Refresh();
		}
	}
}

void Context::setPrevKeyFrame()
{
	Layer* pLayer = layers.getLayer(m_curr_layer);
	if (!pLayer) {
		return;
	}

	KeyFrame* prev = pLayer->getPrevKeyFrame(m_curr_frame);
	if (prev) 
	{
		setCurrFrame(m_curr_layer, prev->getTime());
	}
}

void Context::setNextKeyFrame()
{
	Layer* pLayer = layers.getLayer(m_curr_layer);
	if (!pLayer) {
		return;
	}

	KeyFrame* next = pLayer->getNextKeyFrame(m_curr_frame);
	if (next) 
	{
		setCurrFrame(m_curr_layer, next->getTime());
	}
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}

void Context::reloadViewList(const KeyFrame& frame)
{
	if (!viewlist) return;

	viewlist->clear();
	const std::vector<d2d::ISprite*>& sprites = frame.getAllSprites();
	for (int i = 0, n = sprites.size(); i < n; ++i)
		viewlist->insert(sprites[i]);
}

} // eanim