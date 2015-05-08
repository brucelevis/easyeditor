#ifndef _LIBSHAPE_DRAW_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawPolygonOP : public DrawPolylineOP
{
public:
	DrawPolygonOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	virtual bool onMouseLeftDClick(int x, int y);

protected:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawPolygonOP

}

#endif // _LIBSHAPE_DRAW_POLYGON_OP_H_