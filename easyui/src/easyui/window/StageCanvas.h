#ifndef _EASYUI_WINDOW_STAGE_CANVAS_H_
#define _EASYUI_WINDOW_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace eui
{
namespace window
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}
}

#endif // _EASYUI_WINDOW_STAGE_CANVAS_H_
