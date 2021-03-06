#ifndef _EASYEDITOR_DIALOG_STAGE_CANVAS_H_
#define _EASYEDITOR_DIALOG_STAGE_CANVAS_H_

#include "CameraCanvas.h"

namespace ee
{

class Symbol;

class DialogStageCanvas : public CameraCanvas
{
public:
	DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		Symbol* symbol, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	Symbol* m_symbol;

}; // DialogStageCanvas

}

#endif // _EASYEDITOR_DIALOG_STAGE_CANVAS_H_