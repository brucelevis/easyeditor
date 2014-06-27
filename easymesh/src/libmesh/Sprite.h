#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace emesh
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& s);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* clone() const;

	//
	// ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void loadBodyFromFile();

	Shape* getShape() {
		return m_symbol ? m_symbol->getShape() : NULL; 
	}
	const d2d::Image* getImage() const {
		return m_symbol ? m_symbol->getImage() : NULL;
	}
	d2d::Image* getImage() {
		return m_symbol ? m_symbol->getImage() : NULL;
	}

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_