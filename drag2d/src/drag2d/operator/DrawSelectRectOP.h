#ifndef _DRAG2D_DRAW_SELECT_RECT_OP_
#define _DRAG2D_DRAW_SELECT_RECT_OP_

#include "ZoomViewOP.h"

#include "render/ShapeStyle.h"

namespace d2d
{

class DrawSelectRectOP : public ZoomViewOP
{
public:
	DrawSelectRectOP(EditPanel* editPanel, bool bOpenRightTap = true);
	
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

#endif // _DRAG2D_DRAW_SELECT_RECT_OP_