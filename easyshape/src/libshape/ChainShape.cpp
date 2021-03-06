#include "ChainShape.h"
#include "ChainPropertySetting.h"
#include "PolylineEditor.h"

#include <ee/Math2D.h>
#include <ee/SettingData.h>
#include <ee/EE_RVG.h>

#include <SM_Calc.h>

namespace eshape
{

ChainShape::ChainShape()
{
	m_draw_dir = true;
}

ChainShape::ChainShape(const std::vector<sm::vec2>& vertices, bool closed)
	: s2::PolylineShape(vertices, closed)
	, m_draw_dir(!closed)
{
}

ChainShape* ChainShape::Clone() const
{
	return new ChainShape(*this);
}

void ChainShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);
}

void ChainShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
{
	s2::PolylineShape::Draw(mt, color);

	if (m_draw_dir && m_vertices.size() >= 2) 
	{
		sm::vec2 s = m_vertices[m_vertices.size() - 2],
			     e = m_vertices[m_vertices.size() - 1];
		const float LEN = 20;
		sm::vec2 seg = s - e;
		seg.Normalize();
		seg *= LEN;
		sm::vec2 left, right;
		left = e + sm::rotate_vector(seg, - SM_PI / 6);
		right = e + sm::rotate_vector(seg, SM_PI / 6);
		ee::RVG::Line(e, left); 
		ee::RVG::Line(e, right);
	}
}

ee::PropertySetting* ChainShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new ChainPropertySetting(stage, this);
}

void ChainShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	int num = value["vertices"]["x"].size();
	m_vertices.resize(num);
	for (int i = 0; i < num; ++i) {
		m_vertices[i].x = value["vertices"]["x"][i].asDouble();
		m_vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	m_closed = value["closed"].asBool();
	m_draw_dir = !m_closed;

	UpdateBounding();
}

void ChainShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i) {
		value["vertices"]["x"][i] = m_vertices[i].x;
		value["vertices"]["y"][i] = m_vertices[i].y;
	}
	value["closed"] = m_closed;
}

void ChainShape::AddVertex(int index, const sm::vec2& pos)
{
	PolylineEditor::AddVertex(m_vertices, m_bounding, index, pos);
}

void ChainShape::RemoveVertex(const sm::vec2& pos)
{
	PolylineEditor::RemoveVertex(m_vertices, m_bounding, pos);
}

void ChainShape::ChangeVertex(const sm::vec2& from, const sm::vec2& to)
{
	PolylineEditor::ChangeVertex(m_vertices, m_bounding, from, to);
}

void ChainShape::SetVertices(const std::vector<sm::vec2>& vertices)
{
	m_vertices = vertices;
	UpdateBounding();
}

}