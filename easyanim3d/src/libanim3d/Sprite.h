#ifndef _EASYANIM3D_SPRITE_H_
#define _EASYANIM3D_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace eanim3d
{

class Sprite : public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// Cloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	const sm::vec3& GetPos3() const { return m_pos3; }
	void SetPos3(const sm::vec3& pos) { m_pos3 = pos; }
	void Translate3(const sm::vec3& offset) { m_pos3 += offset; }

	const sm::Quaternion& GetOri3() const { return m_ori3; }
	void SetOri3(const sm::Quaternion& ori) { m_ori3 = ori; }
	void Rotate3(const sm::Quaternion& delta) {
		m_ori3.Rotate(delta);
		m_ori3 = delta.Rotated(m_ori3);
	}

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	sm::vec3 m_pos3;
	sm::Quaternion m_ori3;

}; // Sprite

}

#endif // _EASYANIM3D_SPRITE_H_