#ifndef _DRAG2D_TWO_PASS_CANVAS_H_
#define _DRAG2D_TWO_PASS_CANVAS_H_

#include "OnePassCanvas.h"
#include "IRefreshMonitor.h"

namespace d2d
{

class TwoPassCanvas : public OnePassCanvas, public IRefreshMonitor
{
public:
	TwoPassCanvas(EditPanel* stage);

	//
	// interface IRefreshMonitor
	//
	virtual void OnRefresh();

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

}; // TwoPassCanvas

}

#endif // _DRAG2D_TWO_PASS_CANVAS_H_