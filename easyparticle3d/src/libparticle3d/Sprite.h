#ifndef _EASYPARTICLE3D_SPRITE_H_
#define _EASYPARTICLE3D_SPRITE_H_

#include "Symbol.h"
#include "PS.h"

#include <ee/Sprite.h>

#include <SM_Quaternion.h>

namespace eparticle3d
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
	virtual bool Update(int version);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void Start();

	void Draw(const ee::Matrix& mt) const;

	void SetMatrix(const ee::Matrix& mat);

	sm::Quaternion& GetDir() { return m_dir; }
	const sm::Quaternion& GetDir() const { return m_dir; }

	bool IsLoop() const;
	void SetLoop(bool loop);

	bool IsLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	bool IsAlone() const { return m_alone; }
	void SetAlone(bool alone);

	bool IsReuse() const { return m_reuse; }
	void SetReuse(bool reuse);

	p3d_sprite* GetP3D() const { return m_spr; }

	void OnActive();

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	void CreatePS();

protected:
	Symbol* m_symbol;

private:
	p3d_sprite* m_spr;

	ee::Matrix m_mat;

	sm::Quaternion m_dir;

	bool m_alone;
	bool m_reuse;

	mutable RenderParams m_rp;

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_