#include "Sprite.h"

namespace eicon
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_process(1)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_process(sprite.m_process)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_process(1)
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
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* complex = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && complex)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = complex;
	}
}

void Sprite::load(const Json::Value& val)
{
	ISprite::load(val);
	SetProcess(val["process"].asDouble());
}

void Sprite::store(Json::Value& val) const
{
	ISprite::store(val);
	val["process"] = m_process;
}

void Sprite::SetProcess(float process)
{
	m_process = process;
	buildBounding();
}

}