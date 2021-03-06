#ifndef _EASYICON_SYMBOL_H_
#define _EASYICON_SYMBOL_H_

#include <ee/Symbol.h>

namespace ee { class Image; }

namespace eicon
{

class Icon;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

	void SetIcon(Icon* icon);
	const Icon* GetIcon() const { return m_icon; }
	Icon* GetIcon() { return m_icon; }

	void SetImage(ee::Image* img);

protected:
	virtual void LoadResources();

private:
	Icon* m_icon;

}; // Symbol

}

#endif // _EASYICON_SYMBOL_H_