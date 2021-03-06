#include "CosineShape.h"

#include <ee/SettingData.h>

namespace eshape
{

CosineShape::CosineShape(const std::vector<sm::vec2>& vertices)
	: s2::CosineShape(vertices)
{
}

CosineShape* CosineShape::Clone() const
{
	return new CosineShape(*this);	
}

void CosineShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_mid_points.size(); i < n; ++i) {
		m_mid_points[i] += offset;
	}
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);
}

// void CosineShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
// {
// 	if (m_vertices.empty()) return;
// 
// 	ee::RVG::Color(color.mul);
// 	ee::RVG::Polyline(m_midPoints, m_loop);
// 	if (ee::SettingData::ctl_pos_sz != 0) {
// 		ee::RVG::Color(s2::Color(102, 204, 102));
// 		ee::RVG::Circles(m_vertices, static_cast<float>(ee::SettingData::ctl_pos_sz), true);
// 	}
// 	if (ee::SettingData::ctl_pos_sz != 0) {
// 		ee::RVG::Color(s2::Color(204, 204, 102));
// 		ee::RVG::Circles(m_midPoints, ee::SettingData::ctl_pos_sz * 0.5f, true);
// 	}
// }

}