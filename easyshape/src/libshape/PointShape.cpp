#include "PointShape.h"

namespace libshape
{

PointShape::PointShape()
{
}

PointShape::PointShape(const d2d::Vector& pos)
	: m_pos(pos)
	, m_rect(m_pos, RADIUS, RADIUS)
{
}

PointShape::PointShape(const PointShape& point)
	: m_pos(point.m_pos)
	, m_rect(point.m_rect)
{	
}

PointShape* PointShape::Clone() const
{
	return new PointShape(*this);
}

bool PointShape::IsContain(const d2d::Vector& pos) const
{
	return d2d::Math2D::IsPointInRect(pos, m_rect);
}

bool PointShape::IsIntersect(const d2d::Rect& rect) const
{
	return d2d::Math2D::IsRectIntersectRect(rect, m_rect);
}

void PointShape::Translate(const d2d::Vector& offset)
{
	m_pos += offset;
	m_rect.Translate(offset);
}

void PointShape::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color) const
{
	d2d::PrimitiveDraw::DrawCircle(mt, m_pos, RADIUS, true, 2, color.multi);
}

d2d::IPropertySetting* PointShape::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return NULL;
}

void PointShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::Shape::LoadFromFile(value, dir);

	m_pos.x = value["x"].asDouble();
	m_pos.y = value["y"].asDouble();
}

void PointShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	d2d::Shape::StoreToFile(value, dir);

	value["x"] = m_pos.x;
	value["y"] = m_pos.y;
}

}