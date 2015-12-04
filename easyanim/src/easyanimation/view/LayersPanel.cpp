#include "LayersPanel.h"
#include "LayersPropertyWidget.h"
#include "LayersContentWidget.h"
#include "KeysPanel.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

namespace eanim
{

LayersPanel::LayersPanel(wxWindow* parent)
	: wxPanel(parent)
{
	InitLayout();
}

//void LayersPanel::onPlay()
//{
// 	KeysPanel* keysPanel = m_timeLinePanel->getKeysPanel();
// 	int last = keysPanel->getCurrPos();
// 	do 
// 	{
// 		keysPanel->setCurrPos(++last);
// //		Sleep(1000.0f / 24);
// 	} while (keysPanel->getCurrPos() == last);
//}

void LayersPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_layersProperty = new LayersPropertyWidget(this), 0, wxALIGN_LEFT);
	sizer->Add(m_contentPanel = new LayersContentWidget(this), 1, wxEXPAND);
	SetSizer(sizer);
}

} // eanim