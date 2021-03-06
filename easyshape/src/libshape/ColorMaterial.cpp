#include "ColorMaterial.h"

#include <ee/EE_RVG.h>
#include <ee/Math2D.h>
#include <ee/Triangulation.h>

#include <sprite2/RenderColor.h>

namespace eshape
{

ColorMaterial::ColorMaterial(const std::vector<sm::vec2>& vertices, 
							 const s2::Color& color)
	: m_color(color)
{
	BuildBegin(vertices);
	BuildEnd();
}

Json::Value ColorMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "color";
	val["color"] = m_color.ToRGBA();
	return val;
}

void ColorMaterial::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
{
	std::vector<sm::vec2> tris;
	ee::Math2D::TransVertices(mt, m_tris, tris);
	ee::RVG::Color(m_color * color.mul);
	ee::RVG::Triangles(tris);

	//ee::ShaderMgr::Instance()->RVG();
	//s2::Color c = ee::cMul(m_color, color.mul);
	//ee::PrimitiveDrawRVG::SetColor(ee::trans_color2int(c, ee::PT_ABGR));
	//ee::PrimitiveDrawRVG::Triangles(mt, m_tris);
}

void ColorMaterial::BuildEnd()
{
	m_tris.clear();

	std::vector<sm::vec2> outline;
	ee::Math2D::RemoveDuplicatePoints(m_outline, outline);

	if (!m_segments.empty()) {
		ee::Triangulation::Lines(m_outline, m_segments, m_tris);
	} else if (!m_holes.empty()) {
		ee::Triangulation::Holes(m_outline, m_holes, m_tris);
	} else {
		ee::Triangulation::Normal(m_outline, m_tris);
	}
}

}