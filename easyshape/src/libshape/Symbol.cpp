#include "Symbol.h"
#include "FileAdapter.h"

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

	m_shapes.reserve(symbol.m_shapes.size());
	for (size_t i = 0, n = symbol.m_shapes.size(); i < n; ++i) {
		m_shapes.push_back(symbol.m_shapes[i]->clone());
	}
}

Symbol::~Symbol()
{
	Clear();
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this); 
}

void Symbol::reloadTexture() const
{
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	if (m_bg) {
		m_bg->draw(mt, mul, add, sprite);
	}
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->draw();
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	d2d::Rect rect;
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		rect.combine(m_shapes[i]->getRect());
	return rect;
}

void Symbol::Traverse(d2d::IVisitor& visitor) const
{
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		bool hasNext;
		visitor.visit(m_shapes[i], hasNext);
		if (!hasNext) break;
	}
}

void Symbol::Add(d2d::IShape* shape)
{
	if (shape) {
		shape->Retain();
		m_shapes.push_back(shape);
	}
}

void Symbol::Remove(d2d::IShape* shape)
{
	if (!shape) {
		return;
	}

	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		if (m_shapes[i] == shape) {
			m_shapes.erase(m_shapes.begin() + i);
			shape->Release();
			break;
		}
	}
}

void Symbol::Clear()
{
	if (m_bg) {
		m_bg->Release();
		m_bg = NULL;
	}

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Release();
	}
	m_shapes.clear();
}

void Symbol::SetBG(d2d::ISymbol* bg)
{
	d2d::obj_assign((d2d::Object*&)m_bg, bg);
}

void Symbol::loadResources()
{
	Clear();

	FileAdapter adpater(m_shapes);
	adpater.load(m_filepath.c_str());
}

}