#ifndef _EASYMESH_SHAPE_FACTORY_H_
#define _EASYMESH_SHAPE_FACTORY_H_

#include <drag2d.h>

#include "config.h"

namespace emesh
{

class Shape;

class ShapeFactory
{
public:
	Shape* CreateShape(const d2d::Image& image) const;

	void SetShapeType(ShapeType type);

public:
	static ShapeFactory* Instance();

private:
	ShapeFactory() {}

private:
	ShapeType m_type;

private:
	static ShapeFactory* m_instance;

}; // ShapeFactory

}

#endif // _EASYMESH_SHAPE_FACTORY_H_