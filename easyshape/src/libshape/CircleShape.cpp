#include "CircleShape.h"
#include "CirclePropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

CircleShape::CircleShape()
	: center(0, 0)
	, radius(1)
	, m_rect(center, radius, radius)
{
}

CircleShape::CircleShape(const CircleShape& circle)
{
	center = circle.center;
	radius = circle.radius;
	m_rect = circle.m_rect;
}

CircleShape::CircleShape(const sm::vec2& center, float radius)
	: center(center)
	, radius(radius)
	, m_rect(center, radius, radius)
{
}

CircleShape* CircleShape::Clone() const
{
	return new CircleShape(*this);
}

bool CircleShape::IsContain(const sm::vec2& pos) const
{
	return ee::Math2D::GetDistance(center, pos) < QUERY_ACCURACY;
}

bool CircleShape::IsIntersect(const ee::Rect& rect) const
{
	const float cx = (rect.xmin + rect.xmax) * 0.5f,
		cy = (rect.ymin + rect.ymax) * 0.5f;

	const float dis = ee::Math2D::GetDistance(center, sm::vec2(cx, cy));
	if (dis > sm::vec2(rect.xmax - cx, rect.ymax - cy).Length() + radius)
		return false;

	if (ee::Math2D::IsPointInCircle(sm::vec2(rect.xmin, rect.ymin), center, radius))
		return true;
	if (ee::Math2D::IsPointInCircle(sm::vec2(rect.xmax, rect.ymin), center, radius))
		return true;
	if (ee::Math2D::IsPointInCircle(sm::vec2(rect.xmax, rect.ymax), center, radius))
		return true;
	if (ee::Math2D::IsPointInCircle(sm::vec2(rect.xmin, rect.ymax), center, radius))
		return true;
	if (ee::Math2D::IsPointInRect(center, rect))
		return true;

	if (center.x > rect.xmin && center.x < rect.xmax)
	{
		if (dis < rect.ymax - cy + radius)
			return true;
	}
	if (center.y > rect.ymin && center.y < rect.ymax)
	{
		if (dis < rect.xmax - cx + radius)
			return true;
	}

	return false;
}

void CircleShape::Translate(const sm::vec2& offset)
{
	center += offset;
}

void CircleShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
{
	sm::vec2 c = ee::Math2D::TransVector(center, mt);
	float r = ee::Math2D::TransLen(radius, mt);
	ee::RVG::Color(color.mul);
	ee::RVG::Circle(c, r, false, 32);
}

ee::PropertySetting* CircleShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new CirclePropertySetting(stage, this);
}

void CircleShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	center.x = value["x"].asDouble();
	center.y = value["y"].asDouble();
	radius = value["radius"].asDouble();

	m_rect = ee::Rect(center, radius, radius);
}

void CircleShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	value["x"] = center.x;
	value["y"] = center.y;
	value["radius"] = radius;
}

}