#ifndef _EASYP3DINV_STAGE_PANEL_H_
#define _EASYP3DINV_STAGE_PANEL_H_

#include <drag2d.h>

namespace ep3dinv
{

class StageCanvas;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library, d2d::PropertySettingPanel* property);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

protected:
	virtual void OnSizeDebug(wxSizeEvent& event);

private:
	StageCanvas* m_canvas;

}; // StagePanel

}

#endif // _EASYP3DINV_STAGE_PANEL_H_