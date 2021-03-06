#include "Symbol.h"

#include <ee/FileHelper.h>

#include <easyshape.h>

#include <shaderlab.h>
#include <sprite2/RenderParams.h>


namespace etexture
{

Symbol::Symbol()
{
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
{
	m_shapes.reserve(s.m_shapes.size());
	for (int i = 0, n = s.m_shapes.size(); i < n; ++i) {
		ee::Shape* shape = s.m_shapes[i];
		m_shapes.push_back(shape);
		shape->Retain();
	}
}

Symbol::~Symbol()
{
	Clear();
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Draw(p.mt, p.color);
	}
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->ReloadTexture();
	}
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	sm::rect rect;
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		rect.Combine(m_shapes[i]->GetRect());
	}
	return rect;
}

void Symbol::LoadResources()
{
	Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);

	int i = 0;
	Json::Value shape_val = value["shapes"][i++];
	while (!shape_val.isNull()) {
		ee::Shape* shape = eshape::FileIO::LoadShape(dir, shape_val);
		m_shapes.push_back(shape);
		shape_val = value["shapes"][i++];
	}
}

void Symbol::Clear()
{
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Release();
	}
	m_shapes.clear();
}

}