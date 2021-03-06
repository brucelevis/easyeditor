#ifndef _TEST_DTEX_STAGE_CANVAS_H_
#define _TEST_DTEX_STAGE_CANVAS_H_

#include <ee/OnePassCanvas.h>

namespace tdtex
{

class StagePanel;

class StageCanvas : public ee::OnePassCanvas
{
public:
	StageCanvas(StagePanel* panel);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

private:
	void DrawEJScreen() const;

private:
	StagePanel* m_panel;

}; // StageCanvas

}

#endif // _TEST_DTEX_STAGE_CANVAS_H_