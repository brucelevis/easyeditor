#include "Symbol.h"
#include "FileIO.h"

#include "PolygonShape.h"

#include <easyimage.h>

namespace libshape
{

Symbol::Symbol()
	: m_bg(NULL)
{
}

Symbol::Symbol(const Symbol& symbol)
{
	m_filepath = symbol.m_filepath;

	m_bg = symbol.m_bg;
	if (m_bg) {
		m_bg->Retain();
	}
	m_bg_outline.reserve(symbol.m_bg_outline.size());
	for (size_t i = 0, n = symbol.m_bg_outline.size(); i < n; ++i) {
		m_bg_outline.push_back(symbol.m_bg_outline[i]->Clone());
	}
	
	m_shapes.reserve(symbol.m_shapes.size());
	for (size_t i = 0, n = symbol.m_shapes.size(); i < n; ++i) {
		m_shapes.push_back(symbol.m_shapes[i]->Clone());
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

void Symbol::ReloadTexture() const
{
	if (m_bg) {
		m_bg->ReloadTexture();
	}
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
 	if (m_bg) {
 		m_bg->Draw(mt, color, spr);
 	}
	if (d2d::Config::Instance()->GetSettings().visible_shape)
	{
		for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
			m_bg_outline[i]->Draw(mt);
		}
		for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
			m_shapes[i]->Draw(mt);
		}
	}
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite/* = NULL*/) const
{
	d2d::Rect rect;
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		rect.Combine(m_bg_outline[i]->GetRect());
	}
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		rect.Combine(m_shapes[i]->GetRect());
	}
	return rect;
}

void Symbol::Traverse(d2d::Visitor& visitor) const
{
	for (int i = 0, n = m_bg_outline.size(); i < n; ++i) {
		bool next;
		visitor.Visit(m_bg_outline[i], next);
		if (!next) return;
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		bool next;
		visitor.Visit(m_shapes[i], next);
		if (!next) return;
	}
}

bool Symbol::Add(d2d::Shape* shape)
{
	if (!shape) {
		return false;
	}

	shape->Retain();
	m_shapes.push_back(shape);
	return true;
}

bool Symbol::Remove(d2d::Shape* shape)
{
	if (!shape) {
		return false;
	}

	for (int i = 0, n = m_shapes.size(); i < n; ++i) 
	{
		if (m_shapes[i] == shape) 
		{
			m_shapes.erase(m_shapes.begin() + i);
			shape->Release();
			return true;
		}
	}

	return false;
}

bool Symbol::Clear()
{
	bool ret = !m_bg_outline.empty() || !m_shapes.empty();

	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		m_bg_outline[i]->Release();
	}
	m_bg_outline.clear();

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Release();
	}
	m_shapes.clear();

	return ret;
}

void Symbol::SetBG(d2d::Symbol* bg)
{
	if (m_bg != bg) {
		LoadBGOutline(bg);
		LoadBGTriStrip(bg);
	}
	d2d::obj_assign<d2d::Symbol>(m_bg, bg);
}

void Symbol::StoreToFile(const char* filename) const
{
	std::vector<d2d::Shape*> shapes(m_shapes);
	std::copy(m_bg_outline.begin(), m_bg_outline.end(), back_inserter(shapes));
	FileIO::StoreToFile(filename, shapes, m_bg);
}

void Symbol::LoadResources()
{
	Clear();
	FileIO::LoadFromFile(m_filepath.c_str(), m_shapes, m_bg);
}

void Symbol::LoadBGOutline(d2d::Symbol* bg)
{
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		m_bg_outline[i]->Release();
	}
	m_bg_outline.clear();

	if (!bg) {
		return;
	}

	wxString filepath = d2d::FileHelper::GetFilenameAddTag(
		bg->GetFilepath(), eimage::OUTLINE_FILE_TAG, "json");
	if (!d2d::FileHelper::IsFileExist(filepath)) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::vector<d2d::Vector> vertices;
	d2d::JsonSerializer::Load(value["normal"], vertices);
	if (!vertices.empty()) {
		d2d::Shape* shape = new PolygonShape(vertices);
		m_bg_outline.push_back(shape);
	}
}

void Symbol::LoadBGTriStrip(d2d::Symbol* bg)
{
	m_bg_tri_strips.clear();

	wxString filepath = d2d::FileHelper::GetFilenameAddTag(
		bg->GetFilepath(), eimage::TRI_STRIP_FILE_TAG, "json");
	if (!d2d::FileHelper::IsFileExist(filepath)) {
		return;
	}
	
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value strip_val = value["strips"][i++];
	while (!strip_val.isNull()) {
		std::vector<d2d::Vector> strip;
		d2d::JsonSerializer::Load(strip_val, strip);
		m_bg_tri_strips.push_back(strip);
		strip_val = value["strip"][i++];
	}
}

ShapeType Symbol::GetShapeType() const
{
	if (m_shapes.empty()) {
		return ST_UNKNOWN;
	}

	d2d::Shape* shape = m_shapes[0];
	return get_shape_type(shape->GetShapeDesc());
}

}