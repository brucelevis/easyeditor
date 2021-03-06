#include "Icon.h"

#include <ee/Image.h>
#include <ee/Math2D.h>

#include <shaderlab.h>

namespace eicon
{

Icon::Icon()
	: m_img(NULL)
{
}

Icon::Icon(ee::Image* img)
	: m_img(NULL)
{
	SetImage(img);
}

Icon::~Icon()
{
	if (m_img) {
		m_img->Release();
	}
}

void Icon::Draw(const sm::mat4& mt, float process) const
{
	if (!m_img) {
		return;
	}

	// texid
	int texid = m_img->GetTexID();

	// tex_coords
	sm::vec2 tex_coords[4];
	GetTexCoords(process, tex_coords);

	// vertices
	sm::vec2 scr_coords[4];
	GetScreenCoords(process, tex_coords, scr_coords);
	for (int i = 0; i < 4; ++i) {
		scr_coords[i] = ee::Math2D::TransVector(scr_coords[i], mt);
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->Draw(&scr_coords[0].x, &tex_coords[0].x, texid);
}

sm::rect Icon::GetRegion(float process) const
{
	sm::rect ret;

	if (!m_img) {
		return ret;
	}
	
	sm::vec2 tex_coords[4];
	GetTexCoords(process, tex_coords);

	sm::vec2 scr_coords[4];
	GetScreenCoords(process, tex_coords, scr_coords);
	for (int i = 0; i < 4; ++i) {
		float x = scr_coords[i].x,
			y = scr_coords[i].y;
		if (x < ret.xmin) ret.xmin = x;
		if (x > ret.xmax) ret.xmax = x;
		if (y < ret.ymin) ret.ymin = y;
		if (y > ret.ymax) ret.ymax = y;
	}

	return ret;
}

void Icon::GetTexCoords(float process, sm::vec2* tex_coords) const
{
	sm::vec2 bound[4];
	GetBound(process, bound);
	for (int i = 0; i < 4; ++i) {
		tex_coords[i].x = bound[i].x;
		tex_coords[i].y = bound[i].y;
	}
}

void Icon::GetScreenCoords(float process, const sm::vec2* tex_coords, 
						   sm::vec2* screen_coords) const
{
	float w = static_cast<float>(m_img->GetClippedWidth()),
		  h = static_cast<float>(m_img->GetClippedHeight());	
	for (int i = 0; i < 4; ++i) {
		screen_coords[i].x = (tex_coords[i].x - 0.5f) * w;
		screen_coords[i].y = (tex_coords[i].y - 0.5f) * h;
	}
}

void Icon::ReloadTexture() const
{
	if (m_img) {
		m_img->ReloadTexture();
	}
}

void Icon::SetImage(ee::Image* img)
{
	ee::obj_assign<ee::Image>(m_img, img);
	AfterSetImage();
}

}