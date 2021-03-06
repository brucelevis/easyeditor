#ifndef _EASYEDITOR_FONT_BLANK_SPRITE_H_
#define _EASYEDITOR_FONT_BLANK_SPRITE_H_

#include "Sprite.h"
#include "FontBlankSymbol.h"

#include <sprite2/Color.h>

namespace ee
{

class FontBlankSprite : public Sprite
{
public:
	enum HoriAlignType
	{
		HAT_LEFT = 0,
		HAT_RIGHT = 1,
		HAT_CENTER = 2,
		HAT_AUTO = 3
	};

	enum VertAlignType
	{
		VAT_TOP = 0,
		VAT_BOTTOM = 1,
		VAT_CENTER = 2,
		VAT_AUTO = 3
	};

public:
	FontBlankSprite();
	FontBlankSprite(const FontBlankSprite& sprite);
	FontBlankSprite(FontBlankSymbol* symbol);
	virtual ~FontBlankSprite();

	//
	// Cloneable interface
	//
	virtual FontBlankSprite* Clone() const;

	//
	// Sprite interface
	//
	virtual const FontBlankSymbol& GetSymbol() const;
	virtual void SetSymbol(Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	void LoadFont(const std::string& filename);

	void SetTextContent(const std::string& str) { m_text = str; }
	const std::string& GetTextContext() const { return m_text; }

	void SetTextID(const std::string& tid) { m_tid = tid; }
	const std::string& GetTextID() const { return m_tid; }

	const std::string& GetSymbolName() const { return m_symbol->name; }

protected:
	FontBlankSymbol* m_symbol;

public:
	std::string font;

	bool has_edge;

	s2::Color font_color;

	HoriAlignType align_hori;
	VertAlignType align_vert;
	int size;

	int width, height;

	std::string filename;

private:
	std::string m_text;

	std::string m_tid;

}; // FontBlankSprite
}

#endif // _EASYEDITOR_FONT_BLANK_SPRITE_H_