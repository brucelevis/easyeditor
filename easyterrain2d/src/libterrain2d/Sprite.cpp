#include "Sprite.h"

namespace eterrain2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& s)
	: d2d::ISprite(s)
{
	m_symbol = s.m_symbol->clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	buildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::clone() const
{
	return new Sprite(*this);
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* mesh = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != mesh)
	{
		if (m_symbol) {
			m_symbol->Release();
		}
		symbol->Retain();

		m_symbol = mesh;
		buildBounding();
	}
}

void Sprite::buildBounding()
{
	if (!m_bounding) 
		m_bounding = d2d::BVFactory::createBV(d2d::e_obb);

	d2d::Rect rect = m_symbol->getSize();
	if (m_offset.x == 0 && m_offset.y == 0)
		m_offset.set(rect.xCenter(), rect.yCenter());
	rect.scale(m_scale.x, m_scale.y);
	rect.shear(m_shear.x, m_shear.y);
	m_bounding->initFromRect(rect);
	m_bounding->setTransform(m_pos, m_offset, m_angle);
}

}