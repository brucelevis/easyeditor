#ifndef _EASYTERRAIN2D_SPRITE_H_
#define _EASYTERRAIN2D_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace eterrain2d
{

class Sprite : public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& s);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(const s2::RenderParams& params, float dt) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYTERRAIN2D_SPRITE_H_