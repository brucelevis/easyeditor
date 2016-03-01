#ifndef _EASYEDITOR_DRAW_SELECT_RECT_OP_
#define _EASYEDITOR_DRAW_SELECT_RECT_OP_

#include "ZoomViewOP.h"
#include "Vector.h"
#include "ShapeStyle.h"

namespace ee
{

class DrawSelectRectOP : public ZoomViewOP
{
public:
	DrawSelectRectOP(wxWindow* wnd, EditPanelImpl* stage, 
		bool bOpenRightTap = true);
	
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	Vector m_first_pos, m_last_pos;

	ShapeStyle m_style;

}; // DrawSelectRectOP

}

#endif // _EASYEDITOR_DRAW_SELECT_RECT_OP_