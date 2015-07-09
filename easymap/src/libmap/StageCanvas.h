#ifndef _EASYMAP_STAGE_CANVAS_H_
#define _EASYMAP_STAGE_CANVAS_H_

#include "drag2d.h"

namespace emap
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* panel);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_panel;

}; // StageCanvas

}

#endif // _EASYMAP_STAGE_CANVAS_H_
