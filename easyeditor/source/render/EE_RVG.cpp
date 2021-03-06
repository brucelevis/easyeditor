#include "EE_RVG.h"
#include "CameraMgr.h"

#include <rvg.h>
#include <shaderlab.h>
#include <sprite2/Color.h>

namespace ee
{

void RVG::Init()
{
	rvg_style_init();
}

void RVG::Color(const s2::Color& color)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::Shape2Shader* shader = static_cast<sl::Shape2Shader*>(mgr->GetShader(sl::SHAPE2));
		shader->SetColor(color.ToABGR());
	} else {
		sl::Shape3Shader* shader = static_cast<sl::Shape3Shader*>(mgr->GetShader(sl::SHAPE3));
		shader->SetColor(color.ToABGR());
	}
}

void RVG::PointSize(float size)
{
	rvg_point_size(size);
}

void RVG::LineWidth(float width)
{
	rvg_line_width(width);
}

void RVG::Point(const sm::vec2& vertex)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_point(vertex.x, vertex.y);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_point3(vertex.x, vertex.y, 0);
	}
}

void RVG::Points(const std::vector<sm::vec2>& vertices)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			rvg_point(vertices[i].x, vertices[i].y);
		}
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			rvg_point3(vertices[i].x, vertices[i].y, 0);
		}
	}
}

void RVG::Line(const sm::vec2& p0, const sm::vec2& p1)
{
	rvg_line_style(LS_DEFAULT);

	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::DotLine(const sm::vec2& p0, const sm::vec2& p1)
{
	rvg_line_style(LS_DOT);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::DashLine(const sm::vec2& p0, const sm::vec2& p1)
{
	rvg_line_style(LS_DASH);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::DotDashLine(const sm::vec2& p0, const sm::vec2& p1)
{
	rvg_line_style(LS_DOT_DASH);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
}

void RVG::Lines(const std::vector<sm::vec2>& vertices)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_lines(&vertices[0].x, vertices.size());
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		std::vector<sm::vec3> vertices3;
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			vertices3.push_back(sm::vec3(vertices[i].x, vertices[i].y, 0));
		}
		rvg_lines(&vertices3[0].x, vertices3.size());
	}
}

void RVG::Polyline(const std::vector<sm::vec2>& vertices, bool loop)
{
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_polyline(&vertices[0].x, vertices.size(), loop);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		std::vector<sm::vec3> vertices3;
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			vertices3.push_back(sm::vec3(vertices[i].x, vertices[i].y, 0));
		}
		rvg_polyline3(&vertices3[0].x, vertices3.size(), loop);
	}
}

void RVG::Triangles(const std::vector<sm::vec2>& triangles)
{
	if (triangles.size() < 3) return;

	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_triangles(&triangles[0].x, triangles.size());
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		std::vector<sm::vec3> triangles3;
		for (int i = 0, n = triangles.size(); i < n; ++i) {
			triangles3.push_back(sm::vec3(triangles[i].x, triangles[i].y, 0));
		}
		rvg_triangles3(&triangles3[0].x, triangles3.size());
	}
}

void RVG::Triangles(const std::vector<sm::vec2>& triangles, const std::vector<s2::Color>& colors)
{
	// todo
}

void RVG::TriangleStrip(const std::vector<sm::vec2>& triangles)
{
	if (triangles.size() < 3) return;

	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_triangle_strip(&triangles[0].x, triangles.size());
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		std::vector<sm::vec3> triangles3;
		for (int i = 0, n = triangles.size(); i < n; ++i) {
			triangles3.push_back(sm::vec3(triangles[i].x, triangles[i].y, 0));
		}
		rvg_triangle_strip3(&triangles3[0].x, triangles3.size());
	}
}

void RVG::Rect(const sm::vec2& center, float hw, float hh, bool filling)
{
	Rect(center - sm::vec2(hw, hh), center + sm::vec2(hw, hh), filling);
}

void RVG::Rect(const sm::vec2& p0, const sm::vec2& p1, bool filling)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_rect(p0.x, p0.y, p1.x, p1.y, filling);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_rect3(p0.x, p0.y, p1.x, p1.y, 0, filling);
	}
}

void RVG::Circle(const sm::vec2& center, float radius, bool filling, int segments)
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE2);
		rvg_circle(center.x, center.y, radius, filling, segments);
	} else {
		sl::ShaderMgr::Instance()->SetShader(sl::SHAPE3);
		rvg_circle3(center.x, center.y, 0, radius, filling, segments);
	}
}

void RVG::Circles(const std::vector<sm::vec2>& circles, float radius, bool filling, int segments)
{
	for (int i = 0, n = circles.size(); i < n; ++i) {
		Circle(circles[i], radius, filling, segments);
	}
}

void RVG::Cross(const sm::vec2& center, float edge)
{
	Cross(center, edge, edge);
}

void RVG::Cross(const sm::vec2& center, float xedge, float yedge)
{
	sm::vec2 s = center, e = center;
	s.x -= xedge;
	e.x += xedge;
	Line(s, e);

	s  = e = center;
	s.y -= yedge;
	e.y += yedge;
	Line(s, e);
}

}